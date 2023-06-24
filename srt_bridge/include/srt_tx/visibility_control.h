#ifndef SRT_TX__VISIBILITY_CONTROL_H_
#define SRT_TX__VISIBILITY_CONTROL_H_

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define SRT_TX_EXPORT __attribute__ ((dllexport))
    #define SRT_TX_IMPORT __attribute__ ((dllimport))
  #else
    #define SRT_TX_EXPORT __declspec(dllexport)
    #define SRT_TX_IMPORT __declspec(dllimport)
  #endif
  #ifdef SRT_TX_BUILDING_LIBRARY
    #define SRT_TX_PUBLIC SRT_TX_EXPORT
  #else
    #define SRT_TX_PUBLIC SRT_TX_IMPORT
  #endif
  #define SRT_TX_PUBLIC_TYPE SRT_TX_PUBLIC
  #define SRT_TX_LOCAL
#else
  #define SRT_TX_EXPORT __attribute__ ((visibility("default")))
  #define SRT_TX_IMPORT
  #if __GNUC__ >= 4
    #define SRT_TX_PUBLIC __attribute__ ((visibility("default")))
    #define SRT_TX_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define SRT_TX_PUBLIC
    #define SRT_TX_LOCAL
  #endif
  #define SRT_TX_PUBLIC_TYPE
#endif

#endif  // SRT_TX__VISIBILITY_CONTROL_H_
