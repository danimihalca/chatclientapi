#ifndef WEBSOCKETCLIENT_HPP
#define WEBSOCKETCLIENT_HPP

#include <string>
#include <thread>
#include <mutex>
#include <list>
#include <queue>

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
    void setServer(const std::string& address, uint16_t port);
    void connect();
    void startService();
    void sendText(const std::string& text);
    void closeConnection();
    void addListener(IWebsocketClientListener* listener);
    void removeListener(IWebsocketClientListener* listener);

    //Implements IWebsocketClientCallbackListener
public:
    void onTextReceived(const std::string& message);
    void onConnected();
    void onDisconnected();
    void onConnectionError();

private:
    void run();

private:
    //libwebsockets specific objects
    struct libwebsocket_context* p_context;
    struct libwebsocket_protocols m_protocols[2];
//    struct session_data m_sessionData;
    struct lws_context_creation_info m_info;

    struct user_data m_userData;

    //flow control
    std::thread m_thread;
    std::mutex m_mutex;
    bool b_connected = false;
    bool b_running = false;
    bool b_initialized = false;

    std::list<IWebsocketClientListener*> m_websocketListeners;
    std::string m_serverAddress;
    uint16_t m_ServerPort;

    std::queue<std::string> m_messageQueue;
};



#endif //WEBSOCKETCLIENT_HPP
