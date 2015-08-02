#ifndef WEBSOCKET_CLIENT_CALLBACK_HPP
#define WEBSOCKET_CLIENT_CALLBACK_HPP

#include <libwebsockets.h>

#include "chat_client_common.hpp"

const int MAX_PAYLOAD = 1400;

struct session_data
{
    unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + MAX_PAYLOAD +
                      LWS_SEND_BUFFER_POST_PADDING];
    unsigned int len;
};

LIBRARY_VISIBILITY int websocket_callback(
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
    sizeof(struct session_data),
    MAX_PAYLOAD
};

const struct libwebsocket_protocols null_protocol =
{
    NULL,
    NULL,
    0
};


#endif // WEBSOCKET_CLIENT_CALLBACK_HPP

