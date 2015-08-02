#include "WebsocketCallback/websocket_callback.hpp"
#include "WebsocketCallback/IWebsocketCallbackListener.hpp"

int websocket_callback(libwebsocket_context*         context,
                       libwebsocket*                 wsi,
                       libwebsocket_callback_reasons reason,
                       void*                         user,
                       void*                         in,
                       size_t /*len*/)
{
    struct session_data* pss = (struct session_data*)user;
    IWebsocketCallbackListener* listener =
        (IWebsocketCallbackListener*) libwebsocket_context_user(context);

    switch(reason)
    {

        case LWS_CALLBACK_CLIENT_ESTABLISHED:
        {
            if (listener != nullptr)
            {
                listener->onConnected();
            }
            break;
        }

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        {
            if (listener != nullptr)
            {
                listener->onConnectionError();
            }
            return -1;

            break;
        }

        case LWS_CALLBACK_CLOSED:
        {
            if (listener != nullptr)
            {
                listener->onDisconnected();
            }
            return -1;

            break;
        }

        case LWS_CALLBACK_CLIENT_RECEIVE:
        {
            if(listener != nullptr)
            {
                listener->onMessageReceived((char*)in);
            }
            break;
        }


        case LWS_CALLBACK_CLIENT_WRITEABLE:

        {
            int n = libwebsocket_write(wsi,
                                       &pss->buf[LWS_SEND_BUFFER_PRE_PADDING],
                                       pss->len,
                                       LWS_WRITE_TEXT);
            if(n < 0)
            {
                lwsl_err("ERROR %d writing to socket, hanging up\n", n);
                return -1;
            }
            if(n < (int)pss->len)
            {
                lwsl_err("Partial write\n");
                return -1;
            }
            break;
        }

        default:
        {
            break;
        }

    }

    return 0;
}
