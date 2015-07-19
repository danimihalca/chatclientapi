#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <string>
#include <thread>
#include <list>

#include "WebsocketClient/IWebsocketClient.h"

#include "WebsocketCallback/IWebsocketCallbackListener.h"
#include "WebsocketCallback/websocket_callback.h"

#include "chat_client_common.h"


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
    void addWebsocketClientListener(IWebsocketClientListener* listener);
    void removeWebsocketClientListener(IWebsocketClientListener* listener);

    //Implements IWebsocketClientCallbackListener
public:
    void onMessageReceived(const std::string& message);
    void onConnected();
    void onDisconnected();

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

    std::list<IWebsocketClientListener*> m_websocketListeners;
};



#endif //WEBSOCKETCLIENT_H