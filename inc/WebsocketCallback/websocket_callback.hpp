#ifndef WEBSOCKET_CLIENT_CALLBACK_HPP
#define WEBSOCKET_CLIENT_CALLBACK_HPP

#include <libwebsockets.h>
#include <mutex>
#include <queue>
#include "chat_client_common.hpp"

class IWebsocketCallbackListener;

const int MAX_PAYLOAD = 1400;

struct user_data
{
    IWebsocketCallbackListener* listener;
    std::mutex* mutex;
    std::queue<std::string>* messageQueue;
};

/*LIBRARY_VISIBILITY*/ int websocket_callback(
    struct libwebsocket_context*       context,
    struct libwebsocket*               wsi,
    enum libwebsocket_callback_reasons reason,
    void*                              user,
    void*                              in,
    size_t                             len);

const struct libwebsocket_protocols default_protocol =
{
    "default",
    websocket_callback,
    0,
    MAX_PAYLOAD
};

const struct libwebsocket_protocols null_protocol =
{
    NULL,
    NULL,
    0
};


#endif // WEBSOCKET_CLIENT_CALLBACK_HPP

