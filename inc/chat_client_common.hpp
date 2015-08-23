#ifndef COMMON_H
#define COMMON_H

#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
 #ifdef BUILDING_LIBRARY
  #ifdef __GNUC__
   #define CHATCLIENT_API __attribute__ ((dllexport))
  #else
   #define CHATCLIENT_API __declspec(dllexport)
  #endif
 #else
  #ifdef __GNUC__
   #define CHATCLIENT_API __attribute__ ((dllimport))
  #else
   #define CHATCLIENT_API __declspec(dllimport)
  #endif
 #endif
#else
 #if __GNUC__ >= 4
  #define CHATCLIENT_API __attribute__ ((visibility ("default")))
 #else
  #define CHATCLIENT_API
 #endif
#endif

#endif // COMMON_H
