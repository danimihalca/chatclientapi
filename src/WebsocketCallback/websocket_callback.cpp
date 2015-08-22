#include "WebsocketCallback/websocket_callback.hpp"
#include "WebsocketCallback/IWebsocketCallbackListener.hpp"

#include <debug_utils/log_debug.hpp>

int websocket_callback(libwebsocket_context*         context,
                       libwebsocket*                 wsi,
                       libwebsocket_callback_reasons reason,
                       void*                         user,
                       void*                         in,
                       size_t /*len*/)
{
//    struct session_data* pss = (struct session_data*)user;
    user_data* ud = (user_data*) libwebsocket_context_user(context);


    switch(reason)
    {

        case LWS_CALLBACK_CLIENT_ESTABLISHED:
        {
            if (ud->listener != nullptr)
            {
                ud->listener->onConnected();
            }
            break;
        }

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        {
            if (ud->listener != nullptr)
            {
                ud->listener->onConnectionError();
            }
            return -1;

            break;
        }

        case LWS_CALLBACK_CLOSED:
        {
            if (ud->listener != nullptr)
            {
                ud->listener->onDisconnected();
            }
            return -1;

            break;
        }

        case LWS_CALLBACK_CLIENT_RECEIVE:
        {
            if(ud->listener != nullptr)
            {
                ud->listener->onTextReceived((char*)in);
            }
            LOG_DEBUG_METHOD;
            break;
        }


        case LWS_CALLBACK_CLIENT_WRITEABLE:
        {
            std::lock_guard<std::mutex> lock(*ud->mutex);
            std::string& text = ud->messageQueue->front();
            size_t length = text.length() - LWS_SEND_BUFFER_POST_PADDING -LWS_SEND_BUFFER_PRE_PADDING;
            libwebsocket_write(wsi,
                                       (unsigned char*)&(text.c_str()[LWS_SEND_BUFFER_PRE_PADDING]),
                                       length,
                                       LWS_WRITE_TEXT);
            ud->messageQueue->pop();
//            int n = libwebsocket_write(wsi,
//                                       &pss->buf[LWS_SEND_BUFFER_PRE_PADDING],
//                                       pss->len,
//                                       LWS_WRITE_TEXT);


//            if(n < 0)
//            {
//                lwsl_err("ERROR %d writing to socket, hanging up\n", n);
//                return -1;
//            }
//            if(n < (int)pss->len)
//            {
//                lwsl_err("Partial write\n");
//                return -1;
//            }
            break;
        }

        default:
        {
            break;
        }

    }

    //LOG_DEBUG_METHOD;
    return 0;
}
