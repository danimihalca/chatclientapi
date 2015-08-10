#ifndef WEBSOCKETCLIENT_HPP
#define WEBSOCKETCLIENT_HPP

#include <string>
#include <thread>
#include <list>

#include "WebsocketClient/IWebsocketClient.hpp"

#include "WebsocketCallback/IWebsocketCallbackListener.hpp"
#include "WebsocketCallback/websocket_callback.hpp"

#include "chat_client_common.hpp"


class WebsocketClient :
    public IWebsocketClient,
    public IWebsocketCallbackListener
{

public:
    WebsocketClient();
    ~WebsocketClient();

    //Implements IWebsocketClient
public:
    void initialize();
    void connect(const std::string& address, uint16_t port);
    void startService();
    void sendMessage(const std::string& message);
    void closeConnection();
    void addListener(IWebsocketClientListener* listener);
    void removeListener(IWebsocketClientListener* listener);

    //Implements IWebsocketClientCallbackListener
public:
    void onMessageReceived(const std::string& message);
    void onConnected();
    void onDisconnected();
    void onConnectionError();

private:
    void run();

private:
    //libwebsockets specific objects
    struct libwebsocket_context* p_context;
    struct libwebsocket_protocols m_protocols[2];
    struct session_data data;
    struct lws_context_creation_info m_info;

    //flow control
    std::thread m_thread;
    bool b_connected = false;
    bool b_running = false;
    bool b_initialized = false;
//    bool b_notifiedConnectionError = false;

    std::list<IWebsocketClientListener*> m_websocketListeners;
};



#endif //WEBSOCKETCLIENT_HPP
