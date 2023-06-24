#ifndef SRT_RX__VISIBILITY_CONTROL_H_
#define SRT_RX__VISIBILITY_CONTROL_H_

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define SRT_RX_EXPORT __attribute__ ((dllexport))
    #define SRT_RX_IMPORT __attribute__ ((dllimport))
  #else
    #define SRT_RX_EXPORT __declspec(dllexport)
    #define SRT_RX_IMPORT __declspec(dllimport)
  #endif
  #ifdef SRT_RX_BUILDING_LIBRARY
    #define SRT_RX_PUBLIC SRT_RX_EXPORT
  #else
    #define SRT_RX_PUBLIC SRT_RX_IMPORT
  #endif
  #define SRT_RX_PUBLIC_TYPE SRT_RX_PUBLIC
  #define SRT_RX_LOCAL
#else
  #define SRT_RX_EXPORT __attribute__ ((visibility("default")))
  #define SRT_RX_IMPORT
  #if __GNUC__ >= 4
    #define SRT_RX_PUBLIC __attribute__ ((visibility("default")))
    #define SRT_RX_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define SRT_RX_PUBLIC
    #define SRT_RX_LOCAL
  #endif
  #define SRT_RX_PUBLIC_TYPE
#endif

#endif  // SRT_RX__VISIBILITY_CONTROL_H_
