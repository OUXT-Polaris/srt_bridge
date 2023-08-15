#ifndef _WIN32
   #include <arpa/inet.h>
   #include <netdb.h>
#else
   #include <winsock2.h>
   #include <ws2tcpip.h>
#endif
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <srt.h>

using namespace std;

#include <iostream>
#include <fstream>
#include <cstring>
#include <srt/srt.h>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "usage: receive.cpp <local_ip> <local_port>" << endl;
        return -1;
    }

    srt_startup();

    SRTSOCKET serv = srt_socket(AF_INET, SOCK_DGRAM, 0);

    int rcvbuf_size = 100 * 1024 * 1024;  // 10MB, これはあなたの要件に応じて調整してください。
    if (srt_setsockopt(serv, 0, SRTO_RCVBUF, &rcvbuf_size, sizeof(rcvbuf_size)) == SRT_ERROR) {
        return 2;
    }

    int udp_rcvbuf_size = 100 * 1024 * 1024;  // これも 10MB ですが、必要に応じて変更してください。
    if (srt_setsockopt(serv, 0, SRTO_UDP_RCVBUF, &udp_rcvbuf_size, sizeof(udp_rcvbuf_size)) == SRT_ERROR) {
        return 3;
    }

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

    if (SRT_ERROR == srt_bind(serv, res->ai_addr, res->ai_addrlen))
    {
        cout << "bind: " << srt_getlasterror_str() << endl;
        return -1;
    }

    srt_listen(serv, 10);

    while (true)
    {
        SRTSOCKET clientSock = srt_accept(serv, nullptr, nullptr);
        if (clientSock == SRT_INVALID_SOCK)
            continue;

        // ファイル名の受信
        char filename[1024];
        int len;
        srt_recv(clientSock, (char*)&len, sizeof(int));
        srt_recv(clientSock, filename, len);
        filename[len] = '\0';

        ofstream outFile(filename, ios::binary);
        if (!outFile)
        {
            cout << "Unable to create file: " << filename << endl;
            srt_close(clientSock);
            continue;
        }

        int received = 0;
        char buffer[1024];
        while ((received = srt_recv(clientSock, buffer, sizeof(buffer))) > 0)
        {
            outFile.write(buffer, received);
        }

        outFile.close();
        srt_close(clientSock);
    }

    srt_cleanup();
    return 0;
}

