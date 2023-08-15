#ifndef _WIN32
   #include <cstdlib>
   #include <netdb.h>
#else
   #include <winsock2.h>
   #include <ws2tcpip.h>
#endif
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <srt.h>

using namespace std;

#ifndef _WIN32
void* sendfile(void*);
#else
DWORD WINAPI sendfile(LPVOID);
#endif

#include <iostream>
#include <fstream>
#include <cstring>
#include <srt/srt.h>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        cout << "usage: send.cpp <server_ip> <server_port> <local_filename> <remote_filename>" << endl;
        return -1;
    }

    srt_startup();

    SRTSOCKET serv = srt_socket(AF_INET, SOCK_DGRAM, 0);

    struct addrinfo hints;
    struct addrinfo* res;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (0 != getaddrinfo(argv[1], argv[2], &hints, &res))
    {
        cout << "incorrect server/peer address. " << argv[1] << ":" << argv[2] << endl;
        return -1;
    }

    if (SRT_ERROR == srt_connect(serv, res->ai_addr, res->ai_addrlen))
    {
        cout << "connect: " << srt_getlasterror_str() << endl;
        return -1;
    }

    // ファイル名の送信
    const char* filename = argv[4];
    int len = strlen(filename);
    srt_send(serv, (char*)&len, sizeof(int));
    srt_send(serv, filename, len);

    ifstream file(argv[3], ios::binary);
    if (!file)
    {
        cout << "Unable to open file " << argv[3] << endl;
        srt_close(serv);
        srt_cleanup();
        return -1;
    }

    char buffer[1024];
    while (!file.eof())
    {
        file.read(buffer, sizeof(buffer));
        srt_send(serv, buffer, file.gcount());
    }

    file.close();
    srt_close(serv);
    srt_cleanup();

    return 0;
}


#ifndef _WIN32
void* sendfile(void* usocket)
#else
DWORD WINAPI sendfile(LPVOID usocket)
#endif
{
   SRTSOCKET fhandle = *(SRTSOCKET*)usocket;
   delete (SRTSOCKET*)usocket;

   // Acquiring file name information from client.
   char file[1024];
   int len;

   if (SRT_ERROR == srt_recv(fhandle, (char*)&len, sizeof(int)))
   {
      cout << "recv: " << srt_getlasterror_str() << endl;
      return 0;
   }

   if (SRT_ERROR == srt_recv(fhandle, file, len))
   {
      cout << "recv: " << srt_getlasterror_str() << endl;
      return 0;
   }
   file[len] = '\0';

   // Open the file only to know its size.
   fstream ifs(file, ios::in | ios::binary);
   ifs.seekg(0, ios::end);
   const int64_t size = ifs.tellg();
   ifs.close();

   // Send file size.
   if (SRT_ERROR == srt_send(fhandle, (char*)&size, sizeof(int64_t)))
   {
      cout << "send: " << srt_getlasterror_str() << endl;
      return 0;
   }

   SRT_TRACEBSTATS trace;
   srt_bstats(fhandle, &trace, true);

   // Send the file itself.
   int64_t offset = 0;
   if (SRT_ERROR == srt_sendfile(fhandle, file, &offset, size, SRT_DEFAULT_SENDFILE_BLOCK))
   {
      cout << "sendfile: " << srt_getlasterror_str() << endl;
      return 0;
   }

   srt_bstats(fhandle, &trace, true);
   cout << "speed = " << trace.mbpsSendRate << "Mbits/sec" << endl;
   const int64_t losspercent = 100 * trace.pktSndLossTotal / trace.pktSent;
   cout << "network loss = " << trace.pktSndLossTotal << "pkts (" << losspercent << "%)\n";

   srt_close(fhandle);

   #ifndef _WIN32
      return NULL;
   #else
      return 0;
   #endif
}
