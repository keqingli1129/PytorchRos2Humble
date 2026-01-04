#ifndef CXX_UTILS_PKG__VISIBILITY_CONTROL_H_
#define CXX_UTILS_PKG__VISIBILITY_CONTROL_H_

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define CXX_UTILS_PKG_EXPORT __attribute__ ((dllexport))
    #define CXX_UTILS_PKG_IMPORT __attribute__ ((dllimport))
  #else
    #define CXX_UTILS_PKG_EXPORT __declspec(dllexport)
    #define CXX_UTILS_PKG_IMPORT __declspec(dllimport)
  #endif
  #ifdef CXX_UTILS_PKG_BUILDING_LIBRARY
    #define CXX_UTILS_PKG_PUBLIC CXX_UTILS_PKG_EXPORT
  #else
    #define CXX_UTILS_PKG_PUBLIC CXX_UTILS_PKG_IMPORT
  #endif
  #define CXX_UTILS_PKG_PUBLIC_TYPE CXX_UTILS_PKG_PUBLIC
  #define CXX_UTILS_PKG_LOCAL
#else
  #define CXX_UTILS_PKG_EXPORT __attribute__ ((visibility("default")))
  #define CXX_UTILS_PKG_IMPORT
  #if __GNUC__ >= 4
    #define CXX_UTILS_PKG_PUBLIC __attribute__ ((visibility("default")))
    #define CXX_UTILS_PKG_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define CXX_UTILS_PKG_PUBLIC
    #define CXX_UTILS_PKG_LOCAL
  #endif
  #define CXX_UTILS_PKG_PUBLIC_TYPE
#endif

#endif  // CXX_UTILS_PKG__VISIBILITY_CONTROL_H_
