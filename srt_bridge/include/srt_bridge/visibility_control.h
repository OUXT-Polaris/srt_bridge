#ifndef SRT_BRIDGE__VISIBILITY_CONTROL_H_
#define SRT_BRIDGE__VISIBILITY_CONTROL_H_

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define SRT_BRIDGE_EXPORT __attribute__ ((dllexport))
    #define SRT_BRIDGE_IMPORT __attribute__ ((dllimport))
  #else
    #define SRT_BRIDGE_EXPORT __declspec(dllexport)
    #define SRT_BRIDGE_IMPORT __declspec(dllimport)
  #endif
  #ifdef SRT_BRIDGE_BUILDING_LIBRARY
    #define SRT_BRIDGE_PUBLIC SRT_BRIDGE_EXPORT
  #else
    #define SRT_BRIDGE_PUBLIC SRT_BRIDGE_IMPORT
  #endif
  #define SRT_BRIDGE_PUBLIC_TYPE SRT_BRIDGE_PUBLIC
  #define SRT_BRIDGE_LOCAL
#else
  #define SRT_BRIDGE_EXPORT __attribute__ ((visibility("default")))
  #define SRT_BRIDGE_IMPORT
  #if __GNUC__ >= 4
    #define SRT_BRIDGE_PUBLIC __attribute__ ((visibility("default")))
    #define SRT_BRIDGE_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define SRT_BRIDGE_PUBLIC
    #define SRT_BRIDGE_LOCAL
  #endif
  #define SRT_BRIDGE_PUBLIC_TYPE
#endif

#endif  // SRT_BRIDGE__VISIBILITY_CONTROL_H_
