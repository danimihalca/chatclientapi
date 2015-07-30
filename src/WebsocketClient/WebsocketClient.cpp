#include "WebsocketClient/WebsocketClient.h"
#include "WebsocketClient/IWebsocketClientListener.h"

#include <iostream>
#include <string.h>
#include <sstream>
#include <json/json.h>

#include "log_debug.h"

WebsocketClient::WebsocketClient() :
    m_websocketListeners()
{
    m_protocols[0] = default_protocol;
    m_protocols[1] = null_protocol;
}

void WebsocketClient::initialize()
{

    memset(&m_info, 0, sizeof(m_info));

    m_info.port = CONTEXT_PORT_NO_LISTEN;
    m_info.iface = NULL;
    m_info.protocols = m_protocols;
    m_info.port = 0;
    m_info.user = (IWebsocketCallbackListener*)this;
    m_info.gid = -1;
    m_info.uid = -1;
    m_info.options = 0;
    m_info.http_proxy_address = NULL;

//    m_info.ka_interval =1;
//    m_info.ka_probes = 1;
//    m_info.ka_time=1;

    p_context = libwebsocket_create_context(&m_info);
    b_initialized = true;
}

bool WebsocketClient::connect()
{
    if(!b_initialized)
    {
        initialize();
    }
    b_notifiedConnectionError = false;
    memset(data.buf,0,sizeof(data.buf));
    data.len = 0;
    int use_ssl = 0;
    struct libwebsocket* wsi = libwebsocket_client_connect_extended(p_context,
                                                                    m_address.c_str(),
                                                                    m_port,
                                                                    use_ssl,
                                                                    "/",
                                                                    m_address.c_str(),
                                                                    "origin",
                                                                    NULL,
                                                                    -1,
                                                                    &data);

    if(!wsi)
    {
        log_debug("Client failed to connect to %s:%u\n",
                 m_address.c_str(), m_port);
        onConnectionError();
        return false;
    }
    log_debug("Client connecting to %s:%u\n", m_address.c_str(), m_port);
    startService();
    return true;
}

void WebsocketClient::startService()
{
    if(!b_initialized)
    {
        initialize();
    }
    if (m_thread.joinable())
    {
        m_thread.join();
    }
    b_running = true;
    m_thread = std::thread(&WebsocketClient::run, this);
}


void WebsocketClient::sendMessage(const std::string& message)
{
    log_debug("SENDING : %s\n",message.c_str());
    memset(&data.buf[LWS_SEND_BUFFER_PRE_PADDING],0,MAX_PAYLOAD);
    memcpy(&data.buf[LWS_SEND_BUFFER_PRE_PADDING], message.c_str(),
           strlen(message.c_str()));
    data.len = strlen(message.c_str());

    libwebsocket_callback_on_writable_all_protocol(&m_protocols[0]);
}

void WebsocketClient::closeConnection()
{
    b_running = false;
    if( m_thread.joinable())
    {
        m_thread.join();
    }
    b_initialized = false;
    libwebsocket_context_destroy(p_context);

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
    if(b_connected)
    {
        closeConnection();
    }

}

void WebsocketClient::setServerProperties(const std::string& address,
                                          uint16_t           port)
{
    m_address = address;
    m_port = port;
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
    b_running = false;
}

void WebsocketClient::onConnectionError()
{
    if (!b_notifiedConnectionError)
    {
        for (auto listener: m_websocketListeners)
        {
            listener->onConnectionError();
        }
        b_notifiedConnectionError = true;
        b_running = false;
    }
}

void WebsocketClient::run()
{
    while(b_running)
    {
        int n = libwebsocket_service(p_context, 50);
        if (n < 0)
        {
            b_running = false;
        }
    }
}
