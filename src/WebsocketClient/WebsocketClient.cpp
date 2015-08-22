#include "WebsocketClient/WebsocketClient.hpp"
#include "WebsocketClient/IWebsocketClientListener.hpp"

#include <iostream>
#include <string.h>
#include <sstream>
#include <json/json.h>

#include <debug_utils/log_debug.hpp>

WebsocketClient::WebsocketClient() :
    m_websocketListeners()
{
    m_protocols[0] = default_protocol;
    m_protocols[1] = null_protocol;
    m_userData= user_data{(IWebsocketCallbackListener*)this, &m_mutex,&m_messageQueue};
}

void WebsocketClient::initialize()
{

    memset(&m_info, 0, sizeof(m_info));

    m_info.port = CONTEXT_PORT_NO_LISTEN;
    m_info.iface = NULL;
    m_info.protocols = m_protocols;
    m_info.port = 0;

//    m_userData = new user_data{(IWebsocketCallbackListener*)this, m_mutex};

    m_info.user = &m_userData;
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

void WebsocketClient::setServer(const std::string& address, uint16_t port)
{
    m_serverAddress = address;
    m_ServerPort = port;
}

void WebsocketClient::connect()
{
    if(!b_initialized)
    {
        initialize();
    }
//    b_notifiedConnectionError = false;
    memset(m_sessionData.buf,0,sizeof(m_sessionData.buf));
    m_sessionData.len = 0;
    int use_ssl = 0;
    struct libwebsocket* wsi = libwebsocket_client_connect_extended(p_context,
                                                                    m_serverAddress.c_str(),
                                                                    m_ServerPort,
                                                                    use_ssl,
                                                                    "/",
                                                                    m_serverAddress.c_str(),
                                                                    "origin",
                                                                    NULL,
                                                                    -1,
                                                                    &m_sessionData);

    if(!wsi)
    {
        LOG_DEBUG("Client failed to connect to %s:%u\n",
                 m_serverAddress.c_str(), m_ServerPort);
        onConnectionError();
        return;
    }
    LOG_DEBUG("Client connecting to %s:%u\n", m_serverAddress.c_str(), m_ServerPort);
    startService();
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


void WebsocketClient::sendText(const std::string& text)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    std::string bufferedText = std::string(LWS_SEND_BUFFER_PRE_PADDING,0)+
                               text +
                               std::string(LWS_SEND_BUFFER_POST_PADDING,0);
    LOG_DEBUG("SENDING : %s\n",text.c_str());

    m_messageQueue.push(bufferedText);

//    memset(&m_sessionData.buf[LWS_SEND_BUFFER_PRE_PADDING],0,MAX_PAYLOAD);
//    memcpy(&m_sessionData.buf[LWS_SEND_BUFFER_PRE_PADDING], text.c_str(),
//           strlen(text.c_str()));
//    m_sessionData.len = strlen(text.c_str());

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

void WebsocketClient::addListener(
    IWebsocketClientListener* listener)
{
    m_websocketListeners.push_back(listener);
}

void WebsocketClient::removeListener(
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
//    if (m_userData != nullptr)
//    {
//        delete m_userData;
//    }
}

void WebsocketClient::onTextReceived(const std::string& message)
{
    for (auto listener: m_websocketListeners)
    {
        listener->onTextReceived(message);
    }
    LOG_DEBUG_METHOD;
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
//    if (!b_notifiedConnectionError)
//    {
        for (auto listener: m_websocketListeners)
        {
            listener->onConnectionError();
        }
//        b_notifiedConnectionError = true;
        b_running = false;
//    }
}

void WebsocketClient::run()
{
    while(b_running)
    {
        int n = libwebsocket_service(p_context, 200);
        if (n < 0)
        {
            b_running = false;
        }
    }
}
