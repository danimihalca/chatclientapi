#ifndef CHATCLIENTIMPL_HPP
#define CHATCLIENTIMPL_HPP

#include <memory>
#include <list>

#include "ChatClient/IChatClient.hpp"
#include "WebsocketClient/IWebsocketClientListener.hpp"

class IChatClientListener;
class IActionFactory;
class INotificationParser;
class IWebsocketClient;

class LoginResponseNotification;
class ContactStateChangedNotification;
class ReceiveMessageNotification;
class ReceiveContactsNotification;
class AddRequestNotification;
class AddContactResponseNotification;
class RemovedByContactNotification;

class RegisterUpdateNotification;

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

    void handleLoginResponse(const LoginResponseNotification& loginResponseNotification);
    void handleReceiveContacts(const ReceiveContactsNotification& responseNotification);
    void handleReceiveMessage(const ReceiveMessageNotification& responseNotification);
    void handleContactStateChanged(const ContactStateChangedNotification& responseNotification);

    bool handleAddingByContact(const AddRequestNotification& responseNotification);
    void handleAddContactResponse(const AddContactResponseNotification& responseNotification);
    void handleRemovedByContact(const RemovedByContactNotification& responseNotification);

    void handleRegisterUpdateResponse(const RegisterUpdateNotification& responseNotification);

private:
    Chat_Client_State m_state;
    std::unique_ptr<IWebsocketClient> p_websocketClient;
    std::list<IChatClientListener*> m_clientListeners;
    std::unique_ptr<IActionFactory> p_actionFactory;
    std::unique_ptr<INotificationParser> p_notificationParser;

    UserCredentials* p_userCredentials;
    USER_STATE* p_state;
    User* p_user;
};


#endif //CHATCLIENTIMPL_HPP
