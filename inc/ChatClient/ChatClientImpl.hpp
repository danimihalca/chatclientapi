#ifndef CHATCLIENTIMPL_HPP
#define CHATCLIENTIMPL_HPP

#include <memory>
#include <list>

#include "ChatClient/IChatClient.hpp"
#include "WebsocketClient/IWebsocketClientListener.hpp"

class IChatClientListener;
class IClientJsonFactory;
class IClientJsonParser;
class IWebsocketClient;

class LoginResponseJson;
class ContactStateChangedJson;
class ReceiveMessageJson;
class ReceiveContactsJson;
class AddingByContactJson;
class AddContactResponseJson;
class RemovedByContactJson;

class RegisterUpdateUserResponseJson;

class ChatClientImpl : public IChatClient, public IWebsocketClientListener
{
    enum Chat_Client_State
    {
        INITIAL,
        CONNECTED,
        CONNECTION_ERROR,
        DISCONNECTED,

        LOGGING_IN,
        LOGGED_IN,
        LOG_IN_ERROR,
        REGISTERING
    };

public:
    ChatClientImpl();
    ~ChatClientImpl();

    // Implements IChatClient interface
public:
    virtual void setServer(const std::string& address, uint16_t port);
    virtual void login(const UserCredentials& userCredentials, USER_STATE state);
    virtual void changeState(USER_STATE state);
    virtual void registerUser(const User& user);
    virtual void updateUser(const User& user);
    void sendMessage(int receiverId, const std::string& message);
    void requestContacts();
    void disconnect();
    void addContact(const std::string& userName);
    void removeContact(int contactId);

    void addListener(IChatClientListener* listener);
    void removeListener(IChatClientListener* listener);

    // Implements IWebsocketClientListener interface
public:
    void onTextReceived(const std::string& message);
   virtual void onConnected();
    void onDisconnected();
    virtual void onConnectionError();

private:
    void performLogin();
    void performRegister();

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

    UserCredentials* p_userCredentials;
    USER_STATE* p_state;
    User* p_user;
};


#endif //CHATCLIENTIMPL_HPP
