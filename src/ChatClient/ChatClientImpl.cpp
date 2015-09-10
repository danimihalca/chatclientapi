#include "ChatClient/ChatClientImpl.hpp"
#include "ChatClient/IChatClientListener.hpp"

#include "WebsocketClient/WebsocketClient.hpp"

#include "protocol/JsonActionFactory.hpp"
#include "protocol/JsonNotificationParser.hpp"

#include "protocol/NotificationObject/LoginResponseNotification.hpp"
#include "protocol/NotificationObject/ContactStateChangedNotification.hpp"
#include "protocol/NotificationObject/ReceiveMessageNotification.hpp"
#include "protocol/NotificationObject/ReceiveContactsNotification.hpp"

#include <debug_utils/log_debug.hpp>

#include <Model/Message.hpp>

#include <protocol/ActionObject/IActionObject.hpp>

ChatClientImpl::ChatClientImpl() :
    m_state(INITIAL),
    p_websocketClient(new WebsocketClient()),
    m_clientListeners(),
    p_actionFactory(new JsonActionFactory()),
    p_notificationParser(new JsonNotificationParser())
{
    LOG_DEBUG_METHOD;
    p_websocketClient->addListener(this);
}

ChatClientImpl::~ChatClientImpl()
{
    LOG_DEBUG_METHOD;
    p_websocketClient.reset();
}

void ChatClientImpl::setServer(const std::string& address,
                               uint16_t           port)
{
    LOG_DEBUG_METHOD;
    m_state = INITIAL;
    p_websocketClient->setServer(address, port);
}

void ChatClientImpl::login(const UserCredentials& userCredentials,
                           USER_STATE             state)
{
    LOG_DEBUG_METHOD;
    p_state = new USER_STATE(state);
    p_userCredentials = new UserCredentials(userCredentials);
    if (m_state == CONNECTED ||
        m_state == LOG_IN_ERROR)
    {
        performLogin();
    }
    else if (m_state == INITIAL ||
             m_state == DISCONNECTED ||
             m_state == CONNECTION_ERROR)
    {
        m_state = LOGGING_IN;
        p_websocketClient->connect();
    }

}

void ChatClientImpl::sendMessage(int receiverId, const std::string& message)
{
    LOG_DEBUG_METHOD;
    Message m(message,receiverId);
    IActionObject* action = p_actionFactory->createSendMessageAction(m);
    p_websocketClient->sendText(action->toString());
}

void ChatClientImpl::changeState(USER_STATE state)
{
    LOG_DEBUG_METHOD;
    IActionObject* action = p_actionFactory->createChangeStateAction(state);
    p_websocketClient->sendText(action->toString());
}

void ChatClientImpl::requestContacts()
{
    LOG_DEBUG_METHOD;
    IActionObject* action = p_actionFactory->createRequestContactsAction();

    p_websocketClient->sendText(action->toString());
}

void ChatClientImpl::updateUser(const User& user)
{
    LOG_DEBUG_METHOD;
    IActionObject* action = p_actionFactory->createUpdateUserAction(user);
    p_websocketClient->sendText(action->toString());
}

void ChatClientImpl::registerUser(const User& user)
{
    LOG_DEBUG_METHOD;
    p_user = new User(user);
    if (m_state == CONNECTED ||
        m_state == LOG_IN_ERROR)
    {
        performRegister();
    }
    else if (m_state == INITIAL ||
             m_state == DISCONNECTED ||
             m_state == CONNECTION_ERROR)
    {
        m_state = REGISTERING;
        p_websocketClient->connect();
    }
}

void ChatClientImpl::addListener(IChatClientListener* listener)
{
    LOG_DEBUG_METHOD;
    m_clientListeners.push_back(listener);
}

void ChatClientImpl::removeListener(IChatClientListener* listener)
{
    LOG_DEBUG_METHOD;
    m_clientListeners.remove(listener);
}

void ChatClientImpl::disconnect()
{
    LOG_DEBUG_METHOD;
    p_websocketClient->closeConnection();
}

void ChatClientImpl::addContact(const std::string& userName)
{
    LOG_DEBUG_METHOD;
    IActionObject* action =
        p_actionFactory->createAddContactAction(userName);

    p_websocketClient->sendText(action->toString());

}

void ChatClientImpl::removeContact(int contactId)
{
    LOG_DEBUG_METHOD;
    IActionObject* action = p_actionFactory->createRemoveContactAction(
        contactId);

    p_websocketClient->sendText(action->toString());

}

void ChatClientImpl::onTextReceived(const std::string& message)
{
    LOG_DEBUG_METHOD;
    p_notificationParser->trySetNotificationString(message);
    NOTIFICATION_TYPE action = p_notificationParser->getNotificationType();
    switch (action)
    {

        case NOTIFICATION_LOGIN:
        {
            //            a = p_notificationParser->tryGetLoginResponseAction();
            handleLoginResponse(
                p_notificationParser->tryGetLoginResponseNotification());
            LOG_DEBUG("after resp log\n");
            break;
        }

        case NOTIFICATION_GET_CONTACTS:
        {
            handleReceiveContacts(
                p_notificationParser->tryGetReceiveContactsNotification());
            break;
        }

        case NOTIFICATION_SEND_MESSAGE:
        {
            handleReceiveMessage(
                p_notificationParser->tryGetReceiveMessageNotification());
            break;
        }

        case NOTIFICATION_CONTACT_STATE_CHANGED:
        {
            handleContactStateChanged(
                p_notificationParser->tryGetContactStateChangedNotification());
            break;
        }

        case NOTIFICATION_ADD_REQUEST:
        {

            const AddRequestNotification& notification =
                p_notificationParser->tryGetAddRequestNotification();
            bool accept = handleAddRequest(notification);

            IActionObject* action =
                p_actionFactory->createAddContactResolutionAction(
                    (notification.getUserName()), accept);

            p_websocketClient->sendText(action->toString());
            break;
        }

        case NOTIFICATION_ADD_CONTACT_RESPONSE:
        {
            handleAddContactResponse(
                p_notificationParser->tryGetAddContactResponseNotification());
            //TODO: send add contact notification to listener and wait for result

            break;
        }

        case NOTIFICATION_REMOVE_CONTACT:
        {
            handleRemovedByContact(
                p_notificationParser->tryGetRemovedByContactNotification());
            break;
        }

        case NOTIFICATION_REGISTER_UPDATE_USER:
        {
            handleRegisterUpdateResponse(
                p_notificationParser->tryGetRegisterUpdateUserNotification());
            break;
        }

        default:
        {
            break;
        }
    }
    LOG_DEBUG("end meth\n");
}

void ChatClientImpl::onConnected()
{
    LOG_DEBUG_METHOD;
    if (m_state == LOGGING_IN)
    {
        performLogin();
    }
    if (m_state == REGISTERING)
    {
        performRegister();
    }
    m_state = CONNECTED;
}

void ChatClientImpl::onDisconnected()
{
    LOG_DEBUG_METHOD;
    m_state = DISCONNECTED;
    for (auto listener: m_clientListeners)
    {
        listener->onDisconnected();
    }
}

void ChatClientImpl::onConnectionError()
{
    LOG_DEBUG_METHOD;
    m_state = CONNECTION_ERROR;
    for (auto listener: m_clientListeners)
    {
        listener->onConnectionError();
    }
}

void ChatClientImpl::performLogin()
{
    LOG_DEBUG_METHOD;
    IActionObject* action = p_actionFactory->createLoginAction(
        *p_userCredentials,*p_state);
    p_websocketClient->sendText(action->toString());
    delete p_userCredentials;
    delete p_state;
}

void ChatClientImpl::performRegister()
{
    LOG_DEBUG_METHOD;
    IActionObject* action =
        p_actionFactory->createRegisterUserAction(*p_user);
    p_websocketClient->sendText(action->toString());
    delete p_user;
}

void ChatClientImpl::handleLoginResponse(
    const LoginResponseNotification& responseAction)
{
    LOG_DEBUG_METHOD;

    AUTH_STATUS status = responseAction.getAutheticationStatus();
    if (status == AUTH_SUCCESSFUL)
    {
        m_state = LOGGED_IN;
        for (auto listener: m_clientListeners)
        {
            listener->onLoginSuccessful(
                responseAction.getUserDetails());
        }
    }
    else
    {
        m_state = LOG_IN_ERROR;
        for (auto listener: m_clientListeners)
        {
            listener->onLoginFailed(status);
        }
    }
}

void ChatClientImpl::handleReceiveContacts(
    const ReceiveContactsNotification& responseAction)
{
    LOG_DEBUG_METHOD;
    const std::vector<Contact>& contacts = responseAction.getContacts();
    for(Contact contact: contacts)
    {
        LOG_DEBUG("Contact: %s\n", contact.getUserName().c_str());
    }

    for (auto listener: m_clientListeners)
    {
        listener->onContactsReceived(contacts);
    }
}

void ChatClientImpl::handleReceiveMessage(
    const ReceiveMessageNotification& responseAction)
{
    LOG_DEBUG_METHOD;
    Message message = responseAction.getMessage();
    LOG_DEBUG("M:%s\n",message.getMessageText().c_str());
    for (auto listener: m_clientListeners)
    {
        listener->onMessageReceived(message);
    }
}

void ChatClientImpl::handleContactStateChanged(
    const ContactStateChangedNotification& responseAction)
{
    LOG_DEBUG_METHOD;
    int contactId = responseAction.getContactId();
    USER_STATE contactState = responseAction.getContactState();
    for (auto listener: m_clientListeners)
    {
        listener->onContactStateChanged(contactId, contactState);
    }
}

bool ChatClientImpl::handleAddRequest(
    const AddRequestNotification& responseAction)
{
    LOG_DEBUG_METHOD;
    bool listenerAccepted = false;
    const std::string& requester = responseAction.getUserName();
    for (IChatClientListener* listener: m_clientListeners)
    {
        listenerAccepted = listener->onAddRequest(requester);
        if (listenerAccepted)
        {
            return true;
        }
    }
    return false;
}

void ChatClientImpl::handleAddContactResponse(
    const AddContactResponseNotification& responseAction)
{
    LOG_DEBUG_METHOD;
    const std::string& userName = responseAction.getUserName();
    ADD_STATUS status = responseAction.getStatus();
    for (IChatClientListener* listener: m_clientListeners)
    {
        listener->onAddContactResponse(userName,status);
    }
}

void ChatClientImpl::handleRemovedByContact(
    const RemovedByContactNotification& responseAction)
{
    LOG_DEBUG_METHOD;
    int contactId = responseAction.getContactId();
    for (IChatClientListener* listener: m_clientListeners)
    {
        listener->onRemovedByContact(contactId);
    }
}

void ChatClientImpl::handleRegisterUpdateResponse(
    const RegisterUpdateNotification& responseAction)
{
    LOG_DEBUG_METHOD;
    REGISTER_UPDATE_USER_STATUS status = responseAction.getStatus();
    for (IChatClientListener* listener: m_clientListeners)
    {
        listener->onRegisterUpdateResponse(status);
    }
}
