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

#include <JsonChatProtocol/json_response/LoginResponseJson.hpp>
#include <JsonChatProtocol/json_response/ContactStateChangedJson.hpp>
#include <JsonChatProtocol/json_response/ReceiveMessageJson.hpp>
#include <JsonChatProtocol/json_response/ReceiveContactsJson.hpp>

#include <JsonChatProtocol/json_response/AddingByContactJson.hpp>
#include <JsonChatProtocol/json_response/AddContactResponseJson.hpp>
#include <JsonChatProtocol/json_response/RemovedByContactJson.hpp>

class RegisterUpdateUserResponseJson;

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
    void login(const UserCredentials& userCredentials, USER_STATE state);
    void changeState(USER_STATE state);
    void registerUser(const User& user);
    void updateUser(const User& user);
    void sendMessage(int receiverId, const std::string& message);
    void requestContacts();
    void disconnect();
    void addContact(const std::string& userName);
    void removeContact(int contactId);

    void addListener(IChatClientListener* listener);
    void removeListener(IChatClientListener* listener);

    // Implements IWebsocketClientListener interface
public:
    void onMessageReceived(const std::string& message);
    void onConnected();
    void onDisconnected();
    void onConnectionError();

private:
    void handleLoginResponse(const LoginResponseJson& loginResponseJson);
    void handleReceiveContacts(const ReceiveContactsJson& responseJson);
    void handleReceiveMessage(const ReceiveMessageJson& responseJson);
    void handleContactStateChanged(const ContactStateChangedJson& responseJson);

    bool handleAddingByContact(const AddingByContactJson& responseJson);
    void handleAddContactResponse(const AddContactResponseJson& responseJson);
    void handleRemovedByContact(const RemovedByContactJson& responseJson);

    void handleRegisterUpdateResponse(const RegisterUpdateUserResponseJson& responseJson);

private:
    Chat_Client_State m_state;
    std::unique_ptr<IWebsocketClient> p_websocketClient;
    std::list<IChatClientListener*> m_clientListeners;
    std::unique_ptr<IClientJsonFactory> p_jsonFactory;
    std::unique_ptr<IClientJsonParser> p_jsonParser;

};


#endif //CHATCLIENTIMPL_HPP
