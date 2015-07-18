#include "ChatClientImpl.h"

#include <string.h>


ChatClientImpl::ChatClientImpl()
{
    m_protocols[0].name = "dumb-increment-protocol";
    m_protocols[0].callback = callback2;
    m_protocols[0].per_session_data_size = sizeof(struct session_data);
    m_protocols[0].rx_buffer_size = LWS_SEND_BUFFER_PRE_PADDING + MAX_PAYLOAD +
                                    LWS_SEND_BUFFER_POST_PADDING;

    m_protocols[1].name = NULL;
    m_protocols[1].callback = NULL;
    m_protocols[1].per_session_data_size = 0;
}

void ChatClientImpl::initialize()
{

    memset(&m_info, 0, sizeof(m_info));

    m_info.port = CONTEXT_PORT_NO_LISTEN;
    m_info.iface = NULL;
    m_info.protocols = m_protocols;
    m_info.user = this;
    m_info.gid = -1;
    m_info.uid = -1;
    m_info.options = 0;

    m_context = libwebsocket_create_context(&m_info);
}

void ChatClientImpl::setNewMessageCallback(newMessageCallback callback)
{
    m_newMessageCB = callback;
}

void ChatClientImpl::connect(const std::string& address, uint16_t port)
{
    memset(data.buf,0,sizeof(data.buf));
    data.len = 0;
    int use_ssl = 0;

    struct libwebsocket* wsi = libwebsocket_client_connect_extended(m_context,
                                                                    address.c_str(),
                                                                    port,
                                                                    use_ssl,
                                                                    "/",
                                                                    address.c_str(),
                                                                    "origin",
                                                                    "dumb-increment-protocol",
                                                                    -1,
                                                                    &data);

    if(!wsi)
    {
        lwsl_err("Client failed to connect to %s:%u\n", address.c_str(), port);
    }
    lwsl_notice("Client connecting to %s:%u\n", address.c_str(), port);
}

void ChatClientImpl::startService()
{
    m_running = true;
    m_thread = std::thread(&ChatClientImpl::run, this);
}



void ChatClientImpl::sendMessage(const std::string& message)
{
    while (!m_connected)
    {
    }
    memset(&data.buf[LWS_SEND_BUFFER_PRE_PADDING],0,MAX_PAYLOAD);
    memcpy(&data.buf[LWS_SEND_BUFFER_PRE_PADDING], message.c_str(),
           strlen(message.c_str()));
    data.len = strlen(message.c_str());

    libwebsocket_callback_on_writable_all_protocol(&m_protocols[0]);

}

void ChatClientImpl::closeConnection()
{
    libwebsocket_context_destroy(m_context);
    initialize();
}

ChatClientImpl::~ChatClientImpl()
{
    m_running = false;
    if(m_connected)
    {
        closeConnection();
    }
    if( m_thread.joinable())
    {
        m_thread.join();
    }

}

int ChatClientImpl::callback(libwebsocket_context*         context,
                             libwebsocket*                 wsi,
                             libwebsocket_callback_reasons reason,
                             void*                         user,
                             void*                         in,
                             size_t                        len)
{
    return 0;
}

void ChatClientImpl::run()
{
    while(m_running)
    {
        libwebsocket_service(m_context, 10);
    }
}

int callback2(libwebsocket_context*         context,
              libwebsocket*                 wsi,
              libwebsocket_callback_reasons reason,
              void*                         user,
              void*                         in,
              size_t                        len)
{
    struct session_data* pss = (struct session_data*)user;
    int n;
    if(reason == 23)
    {
        printf("%d\n",reason);
    }
    switch(reason)
    {
    case LWS_CALLBACK_GET_THREAD_ID:
        break;

    case LWS_CALLBACK_CLIENT_ESTABLISHED:
    {
        lwsl_notice("Client has connected\n");
        //            pss->index = 0;
        ((ChatClientImpl*) libwebsocket_context_user(context))->m_connected =
            true;
        break;
    }

    case LWS_CALLBACK_CLIENT_RECEIVE:
        lwsl_notice("Client RX: %s", (char*)in);
        if(((ChatClientImpl*) libwebsocket_context_user(context))->
           m_newMessageCB != nullptr )
        {
            ((ChatClientImpl*) libwebsocket_context_user(context))->
            m_newMessageCB((char*)in);
        }
        break;

    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        lwsl_notice("Client ERROR");
        break;

    case LWS_CALLBACK_CLIENT_WRITEABLE:

        lwsl_notice("Client TX: %s", &pss->buf[LWS_SEND_BUFFER_PRE_PADDING]);
        n = libwebsocket_write(wsi,
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

    default:
        break;
    }

    return 0;
}




