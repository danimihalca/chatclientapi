#ifndef COMMON_H
#define COMMON_H

#if defined (_WIN32)
  #if defined (EXPORT_LIBRARY)
    #define DLL_VISIBILITY __declspec(dllexport)
  #else
    #define DLL_VISIBILITY  __declspec(dllimport)
  #endif // EXPORT_LIBRARY
#endif // _WIN32

typedef void (*newMessageCallback)(const char*);

DLL_VISIBILITY int callback2(struct libwebsocket_context *context,
             struct libwebsocket *wsi,
             enum libwebsocket_callback_reasons reason, void *user,
             void *in, size_t len);

#endif // COMMON_H

