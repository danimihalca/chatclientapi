#ifndef CHATCLIENTIMPL_HPP
#define CHATCLIENTIMPL_HPP

#include <memory>
#include <list>

#include <Model/User.hpp>

#include "ChatClient/IChatClient.hpp"
#include "WebsocketClient/IWebsocketClientListener.hpp"

class IChatClientListener;
class IClientJsonFactory;
class IClientJsonParser;
class IWebsocketClient;


class ChatClientImpl :
    public IChatClient,
    public IWebsocketClientListener
{
    enum Chat_Client_State
    {
        INITIAL,
        CONNECTING,
        CONNECTED,
        CONNECT_ERROR,
        DISCONNECTED,

        LOGGING_IN,
        LOGGED_IN,
        LOG_IN_ERROR
    };

public:
    ChatClientImpl();
    ~ChatClientImpl();

    // Implements IChatClient interface
public:
    void setServerProperties(const std::string& address, uint16_t port);
    void login(const std::string& user, const std::string& password);
    void sendMessage(int receiverId, const std::string& message);
    void getContacts();
    void disconnect();
    void addListener(IChatClientListener* listener);
    void removeListener(IChatClientListener* listener);

    // Implements IWebsocketClientListener interface
public:
    void onMessageReceived(const std::string& message);
    void onConnected();
    void onDisconnected();
    void onConnectionError();

private:
    void performLogin();
    void handleLoginResponse();
    void handleGetContactsResponse();
    void handleReceiveMessage();
    void handleContactOnlineStatusChanged(bool isOnline);

private:
    User m_user;
    Chat_Client_State m_state;
    std::unique_ptr<IWebsocketClient> p_websocketClient;
    std::list<IChatClientListener*> m_clientListeners;
    std::unique_ptr<IClientJsonFactory> p_jsonFactory;
    std::unique_ptr<IClientJsonParser> p_jsonParser;
};


#endif //CHATCLIENTIMPL_HPP
