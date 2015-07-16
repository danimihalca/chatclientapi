#ifndef COMMON_H
#define COMMON_H

#include <libwebsockets.h>


#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
  #ifdef BUILDING_LIBRARY
    #ifdef __GNUC__
      #define LIBRARY_VISIBILITY __attribute__ ((dllexport))
    #else
      #define LIBRARY_VISIBILITY __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define LIBRARY_VISIBILITY __attribute__ ((dllimport))
    #else
      #define LIBRARY_VISIBILITY __declspec(dllimport)
    #endif
  #endif
#else
  #if __GNUC__ >= 4
    #define LIBRARY_VISIBILITY __attribute__ ((visibility ("default")))
  #else
    #define LIBRARY_VISIBILITY
  #endif
#endif

typedef void (*newMessageCallback)(const char*);

LIBRARY_VISIBILITY int callback2(struct libwebsocket_context *context,
                  struct libwebsocket *wsi,
                  enum libwebsocket_callback_reasons reason, void *user,
                  void *in, size_t len);

#endif // COMMON_H

