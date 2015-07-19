#include "WebsocketClient/WebsocketClient.h"
#include "WebsocketClient/IWebsocketClientListener.h"

#include <string.h>


WebsocketClient::WebsocketClient() :
    m_protocols{default_protocol, null_protocol},
    m_websocketListeners()
{
}

void WebsocketClient::initialize()
{

    memset(&m_info, 0, sizeof(m_info));

    m_info.port = CONTEXT_PORT_NO_LISTEN;
    m_info.iface = NULL;
    m_info.protocols = m_protocols;
    m_info.user = this;
    m_info.gid = -1;
    m_info.uid = -1;
    m_info.options = 0;

    p_context = libwebsocket_create_context(&m_info);
    b_initialized = true;
}

void WebsocketClient::connect(const std::string& address, uint16_t port)
{
    if(!b_initialized)
    {
        initialize();
    }
    memset(data.buf,0,sizeof(data.buf));
    data.len = 0;
    int use_ssl = 0;

    struct libwebsocket* wsi = libwebsocket_client_connect_extended(p_context,
                                                                    address.c_str(),
                                                                    port,
                                                                    use_ssl,
                                                                    "/",
                                                                    address.c_str(),
                                                                    "origin",
                                                                    NULL,
                                                                    -1,
                                                                    &data);

    if(!wsi)
    {
        lwsl_err("Client failed to connect to %s:%u\n", address.c_str(), port);
    }
    lwsl_notice("Client connecting to %s:%u\n", address.c_str(), port);
}

void WebsocketClient::startService()
{
    if(!b_initialized)
    {
        initialize();
    }
    m_thread = std::thread(&WebsocketClient::run, this);
    b_running = true;
}



void WebsocketClient::sendMessage(const std::string& message)
{
//    while (!b_connected)
//    {
//    }
    memset(&data.buf[LWS_SEND_BUFFER_PRE_PADDING],0,MAX_PAYLOAD);
    memcpy(&data.buf[LWS_SEND_BUFFER_PRE_PADDING], message.c_str(),
           strlen(message.c_str()));
    data.len = strlen(message.c_str());

    libwebsocket_callback_on_writable_all_protocol(&m_protocols[0]);

}

void WebsocketClient::closeConnection()
{
    libwebsocket_context_destroy(p_context);
    b_initialized = false;
}

void WebsocketClient::addWebsocketClientListener(
    IWebsocketClientListener* listener)
{
    m_websocketListeners.push_back(listener);
}

void WebsocketClient::removeWebsocketClientListener(
    IWebsocketClientListener* listener)
{
    m_websocketListeners.remove(listener);
}

WebsocketClient::~WebsocketClient()
{
    b_running = false;
    if(b_connected)
    {
        closeConnection();
    }
    if( m_thread.joinable())
    {
        m_thread.join();
    }
}

void WebsocketClient::onMessageReceived(const std::string& message)
{
    for (auto listener: m_websocketListeners)
    {
        listener->onMessageReceived(message);
    }
}

void WebsocketClient::onConnected()
{
    b_connected = true;

    for (auto listener: m_websocketListeners)
    {
        listener->onConnected();
    }
}

void WebsocketClient::onDisconnected()
{
    b_connected = false;

    for (auto listener: m_websocketListeners)
    {
        listener->onDisconnected();
    }
}

void WebsocketClient::run()
{
    while(b_running)
    {
        //TODO timeout to 0, sleep afterwards
        libwebsocket_service(p_context, 10);
    }
}