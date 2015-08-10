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

class IResponseJson;
class LoginResponseJson;
class ContactStateChangedJson;
class ReceiveMessageJson;
class ReceiveContactsJson;

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
    void connect(const std::string& address, uint16_t port);
    void login(const UserCredentials& userCredentials);
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
    void handleLoginResponse(LoginResponseJson* loginResponseJson);
    void handleReceiveContacts(ReceiveContactsJson* responseJson);
    void handleReceiveMessage(ReceiveMessageJson* responseJson);
    void handleContactStateChanged(ContactStateChangedJson* responseJson);

private:
    Chat_Client_State m_state;
    std::unique_ptr<IWebsocketClient> p_websocketClient;
    std::list<IChatClientListener*> m_clientListeners;
    std::unique_ptr<IClientJsonFactory> p_jsonFactory;
    std::unique_ptr<IClientJsonParser> p_jsonParser;
};


#endif //CHATCLIENTIMPL_HPP
