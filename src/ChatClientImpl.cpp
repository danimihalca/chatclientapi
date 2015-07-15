#include "ChatClientImpl.h"

#include <string.h>
#include <initializer_list>

#ifdef ANDROID
#include "android/log.h"
#endif

#include <functional>
#include <thread>

void ChatClientImpl::initialize()
{
    bool use_ssl=false;
    using namespace std::placeholders;

    //    std::function<int (struct libwebsocket_context *,
    //                       struct libwebsocket *,
    //                       enum libwebsocket_callback_reasons , void *,
    //                       void *, size_t)> f =
    //            std::bind(&ChatClientImpl::callback, this, _1, _2, _3, _4, _5, _6);

    std::function<callback_function> f =
            std::bind(&ChatClientImpl::callback, this, _1, _2, _3, _4, _5, _6);
    m_protocols = new struct libwebsocket_protocols[2];
    m_protocols[0].name= "default";
    m_protocols[0].callback = callback2;
    m_protocols[0].per_session_data_size = sizeof(struct per_session_data__echo);
    m_protocols[0].rx_buffer_size =LWS_SEND_BUFFER_PRE_PADDING + MAX_ECHO_PAYLOAD + LWS_SEND_BUFFER_POST_PADDING ;

    m_protocols[1].name= NULL;
    m_protocols[1].callback = NULL;
    m_protocols[1].per_session_data_size = 0;

//            /* first protocol must always be HTTP handler */
//    {
//        {
//            "default",		/* name */
//            //            f.target<int (struct libwebsocket_context *,
//            //                           struct libwebsocket *,
//            //                           enum libwebsocket_callback_reasons , void *,
//            //                           void *, size_t)>(),		/* callback */
//            //                    sizeof(struct per_session_data__echo)	/* per_session_data_size */
//            //            f.target<callback_function>(),		/* callback */
//            callback2,
//                    sizeof(struct per_session_data__echo)	/* per_session_data_size */
//        },
//        {
//            NULL, NULL, 0		/* End of list */
//        }
//    };



    memset(&m_info, 0, sizeof m_info);

    m_info.port = CONTEXT_PORT_NO_LISTEN;
    m_info.iface = NULL;
    m_info.protocols = m_protocols;
#ifndef LWS_NO_EXTENSIONS
    m_info.extensions = libwebsocket_get_internal_extensions();
#endif
    if (use_ssl) {
        //        info.ssl_cert_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
        //        info.ssl_private_key_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
    }
    m_info.gid = -1;
    m_info.uid = -1;
    m_info.options = 0;

    m_context = libwebsocket_create_context(&m_info);
}

void ChatClientImpl::setNewMessageCallback(newMessageCallback callback)
{
    m_newMessageCB = callback;
}

void ChatClientImpl::connect(const std::string& uri)
{
    data.index = 73;
    data.client = this;
    memset(data.buf,0,sizeof(data.buf));
    data.len = 0;
    unsigned int port = 9004;
    int use_ssl = 0;
    lwsl_notice("Client connecting to %s:%u....\n", uri.c_str(), port);
    /* we are in client mode */
    //    wsi = libwebsocket_client_connect(m_context, uri.c_str(),
    //                                      port, use_ssl, "/", uri.c_str(),
    //                                      "origin", NULL, -1);
    wsi = libwebsocket_client_connect_extended(m_context, uri.c_str(),
                                               port, use_ssl, "/", uri.c_str(),
                                               "origin", NULL, -1,&data);
//    wsi = libwebsocket_client_connect(m_context, uri.c_str(),
//                                               port, use_ssl, "/", uri.c_str(),
//                                               "origin", NULL, -1);
    if (!wsi) {
        lwsl_err("Client failed to connect to %s:%u\n", uri.c_str(), port);
    }
    lwsl_notice("Client still connecting to %s:%u\n", uri.c_str(), port);
//    wsi->u.ws.rx_user_buffer = malloc(n);
//            if (!wsi->u.ws.rx_user_buffer) {
//                    lwsl_err("Out of Mem allocating rx buffer %d\n", n);
//            }
            //    while (!m_connected)
    //    {
    //        int n = libwebsocket_service(m_context, 10);
    //        lwsl_notice("libwebsocket_service :%d\n",n);
    //    }
}

void ChatClientImpl::startService()
{
    m_running = true;
    m_thread= std::thread(&ChatClientImpl::run, this);
}



void ChatClientImpl::sendMessage(const std::string& message)
{
    data.index -=3;
    memset(&data.buf[LWS_SEND_BUFFER_PRE_PADDING],0,MAX_ECHO_PAYLOAD);
    memcpy(&data.buf[LWS_SEND_BUFFER_PRE_PADDING], message.c_str(),strlen(message.c_str()));
    data.len = strlen(message.c_str());
//    data.len = sprintf((char *)&data.buf[LWS_SEND_BUFFER_PRE_PADDING], "hello from libwebsockets-test-echo client pid %d index %d\n", getpid(), pss->index++);
    libwebsocket_callback_on_writable_all_protocol(&m_protocols[0]);
    //    int n = libwebsocket_service(m_context, 10);
    //    lwsl_notice("libwebsocket_service :%d\n",n);

}

void ChatClientImpl::closeConnection()
{
    //    libwebsocket_cancel_service(m_context);
    //    libwebsocket_

}

ChatClientImpl::~ChatClientImpl()
{
    m_running = false;
    if (m_connected)
    {
        closeConnection();
    }
    if (m_thread.joinable())
    {
        m_thread.join();
    }
    libwebsocket_context_destroy(m_context);

}

int ChatClientImpl::callback(libwebsocket_context* context, libwebsocket* wsi, libwebsocket_callback_reasons reason, void* user, void* in, size_t len)
{
    return 0;
}

void ChatClientImpl::run()
{
    while (m_running)
    {
        libwebsocket_service(m_context, 10);
    }
}

int callback2(libwebsocket_context* context, libwebsocket* wsi, libwebsocket_callback_reasons reason, void* user, void* in, size_t len)
{
    struct per_session_data__echo *pss = (struct per_session_data__echo *)user;
    int n;

    switch (reason) {


        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            lwsl_notice("Client has connected\n");
            //            pss->index = 0;
            ((ChatClientImpl*) pss->client)->m_connected = true;
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            lwsl_notice("Client RX: %s", (char *)in);
            if (((ChatClientImpl*) pss->client)->m_newMessageCB != nullptr)
            {
                ((ChatClientImpl*) pss->client)->m_newMessageCB((char *)in);
            }
            break;
      case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
          lwsl_notice("Client ERROR");
          break;
        case LWS_CALLBACK_CLIENT_WRITEABLE:
            /* we will send our packet... */
//            pss->len = sprintf((char *)&pss->buf[LWS_SEND_BUFFER_PRE_PADDING], "hello from libwebsockets-test-echo client pid %d index %d\n", getpid(), pss->index++);
            lwsl_notice("Client TX: %s", &pss->buf[LWS_SEND_BUFFER_PRE_PADDING]);
            n = libwebsocket_write(wsi, &pss->buf[LWS_SEND_BUFFER_PRE_PADDING], pss->len, LWS_WRITE_TEXT);
            if (n < 0) {
                lwsl_err("ERROR %d writing to socket, hanging up\n", n);
                return -1;
            }
            if (n < (int)pss->len) {
                lwsl_err("Partial write\n");
                return -1;
            }
            break;
        default:
            break;
    }

    return 0;
}




