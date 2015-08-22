#include "ChatClient/ChatClientImpl.hpp"
#include "ChatClient/IChatClientListener.hpp"

#include "WebsocketClient/WebsocketClient.hpp"

#include "JsonProtocol/ClientJsonFactory.hpp"
#include "JsonProtocol/ClientJsonParser.hpp"

#include "JsonChatProtocol/json_response/LoginResponseJson.hpp"
#include "JsonChatProtocol/json_response/ContactStateChangedJson.hpp"
#include "JsonChatProtocol/json_response/ReceiveMessageJson.hpp"
#include <JsonChatProtocol/json_response/ReceiveContactsJson.hpp>

#include <debug_utils/log_debug.hpp>

#include <Model/Message.hpp>

#include <JsonProtocol/ActionJsonObject/IActionJsonObject.hpp>

ChatClientImpl::ChatClientImpl() :
    m_state(INITIAL),
    p_websocketClient(new WebsocketClient()),
    m_clientListeners(),
    p_jsonFactory(new ClientJsonFactory()),
    p_jsonParser(new ClientJsonParser())
{
    p_websocketClient->addListener(this);
}

ChatClientImpl::~ChatClientImpl()
{
    p_websocketClient.reset();
}

void ChatClientImpl::setServer(const std::string& address,
                             uint16_t           port)
{
    p_websocketClient->setServer(address, port);
}

void ChatClientImpl::login(const UserCredentials& userCredentials,
                           USER_STATE             state)
{
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
        p_websocketClient->connect();
        m_state = LOGGING_IN;
    }

}

void ChatClientImpl::sendMessage(int receiverId, const std::string& message)
{
    Message m(message,receiverId);
    IActionJsonObject* json = p_jsonFactory->createSendMessageJson(m);
    p_websocketClient->sendText(json->toString());
}

void ChatClientImpl::changeState(USER_STATE state)
{
   IActionJsonObject* json  = p_jsonFactory->createChangeStateJson(state);
    p_websocketClient->sendText(json->toString());
}

void ChatClientImpl::requestContacts()
{
    IActionJsonObject* json = p_jsonFactory->createRequestContactsJson();

    p_websocketClient->sendText(json->toString());
}

void ChatClientImpl::updateUser(const User& user)
{
   IActionJsonObject* json  = p_jsonFactory->createUpdateUserJson(user);
    p_websocketClient->sendText(json->toString());
}

void ChatClientImpl::registerUser(const User& user)
{
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
        p_websocketClient->connect();
        m_state = REGISTERING;
    }
}

void ChatClientImpl::addListener(IChatClientListener* listener)
{
    m_clientListeners.push_back(listener);
}

void ChatClientImpl::removeListener(IChatClientListener* listener)
{
    m_clientListeners.remove(listener);
}

void ChatClientImpl::disconnect()
{
    p_websocketClient->closeConnection();
}

void ChatClientImpl::addContact(const std::string& userName)
{
   IActionJsonObject* json =
        p_jsonFactory->createAddContactJson(userName);

    p_websocketClient->sendText(json->toString());

}

void ChatClientImpl::removeContact(int contactId)
{
   IActionJsonObject* json  = p_jsonFactory->createRemoveContactJson(
        contactId);

    p_websocketClient->sendText(json->toString());

}

void ChatClientImpl::onTextReceived(const std::string& message)
{
    p_jsonParser->trySetJsonString(message);
    NOTIFICATION_TYPE action = p_jsonParser->getActionType();
    switch (action)
    {

        case NOTIFICATION_LOGIN:
        {
            //            a = p_jsonParser->tryGetLoginResponseJson();
            handleLoginResponse(p_jsonParser->tryGetLoginResponseJson());
            LOG_DEBUG("after resp log\n");
            break;
        }

        case NOTIFICATION_GET_CONTACTS:
        {
            handleReceiveContacts(p_jsonParser->tryGetReceiveContactsJson());
            break;
        }

        case NOTIFICATION_SEND_MESSAGE:
        {
            handleReceiveMessage(p_jsonParser->tryGetReceiveMessageJson());
            break;
        }

        case NOTIFICATION_CONTACT_STATE_CHANGED:
        {
            handleContactStateChanged(
                p_jsonParser->tryGetContactStateChangedJson());
            break;
        }

        case NOTIFICATION_ADD_CONTACT:
        {

            const AddingByContactJson& responseJson =
                p_jsonParser->tryGetAddingByContactJson();
            bool accept = handleAddingByContact(responseJson);

           IActionJsonObject* json  =
                p_jsonFactory->createAddContactResolutionJson(
                    (responseJson.getUserName()), accept);

            p_websocketClient->sendText(json->toString());
            break;
        }

        case NOTIFICATION_ADD_CONTACT_RESOLUTION:
        {
            handleAddContactResponse(
                p_jsonParser->tryGetAddContactResponseJson());
            //TODO: send add contact notification to listener and wait for result

            break;
        }

        case NOTIFICATION_REMOVE_CONTACT:
        {
            handleRemovedByContact(p_jsonParser->tryGetRemovedByContactJson());
            break;
        }

        case NOTIFICATION_REGISTER_UPDATE_USER:
        {
            handleRegisterUpdateResponse(
                p_jsonParser->tryGetRegisterUpdateUserJson());
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
    m_state = DISCONNECTED;
    for (auto listener: m_clientListeners)
    {
        listener->onDisconnected();
    }
}

void ChatClientImpl::onConnectionError()
{
    m_state = CONNECTION_ERROR;
    for (auto listener: m_clientListeners)
    {
        listener->onConnectionError();
    }
}

void ChatClientImpl::performLogin()
{
    IActionJsonObject* json = p_jsonFactory->createLoginJson(
        *p_userCredentials,*p_state);
    p_websocketClient->sendText(json->toString());
    delete p_userCredentials;
    delete p_state;
}

void ChatClientImpl::performRegister()
{
    IActionJsonObject* json  =
        p_jsonFactory->createRegisterUserJson(*p_user);
    p_websocketClient->sendText(json->toString());
    delete p_user;
}

void ChatClientImpl::handleLoginResponse(const LoginResponseJson& responseJson)
{

    AUTH_STATUS status = responseJson.getAutheticationStatus();
    if (status == AUTH_SUCCESSFUL)
    {
        m_state = LOGGED_IN;
        for (auto listener: m_clientListeners)
        {
            listener->onLoginSuccessful(
                responseJson.getUserDetails());
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
    const ReceiveContactsJson& responseJson)
{
    const std::vector<Contact>& contacts = responseJson.getContacts();
    for(Contact contact: contacts)
    {
        LOG_DEBUG("Contact: %s\n", contact.getUserName().c_str());
    }

    for (auto listener: m_clientListeners)
    {
        listener->onContactsReceived(contacts);
    }
}

void ChatClientImpl::handleReceiveMessage(const ReceiveMessageJson& responseJson)
{
    Message message = responseJson.getMessage();
    LOG_DEBUG("M:%s\n",message.getMessageText().c_str());
    for (auto listener: m_clientListeners)
    {
        listener->onMessageReceived(message);
    }
}

void ChatClientImpl::handleContactStateChanged(
    const ContactStateChangedJson& responseJson)
{
    int contactId = responseJson.getContactId();
    USER_STATE contactState = responseJson.getContactState();
//    LOG_DEBUG("%d %d\n",userId,jsonObject);
    for (auto listener: m_clientListeners)
    {
        listener->onContactStateChanged(contactId, contactState);
    }
}

bool ChatClientImpl::handleAddingByContact(
    const AddingByContactJson& responseJson)
{
    bool listenerAccepted = false;
    const std::string& requester = responseJson.getUserName();
    for (IChatClientListener* listener: m_clientListeners)
    {
        listenerAccepted = listener->onAddingByContact(requester);
        if (listenerAccepted)
        {
            return true;
        }
    }
    return false;
}

void ChatClientImpl::handleAddContactResponse(
    const AddContactResponseJson& responseJson)
{
    const std::string& userName = responseJson.getUserName();
    ADD_STATUS status = responseJson.getStatus();
    for (IChatClientListener* listener: m_clientListeners)
    {
        listener->onAddContactResponse(userName,status);
    }
}

void ChatClientImpl::handleRemovedByContact(
    const RemovedByContactJson& responseJson)
{
    int contactId = responseJson.getContactId();
    for (IChatClientListener* listener: m_clientListeners)
    {
        listener->onRemovedByContact(contactId);
    }
}

void ChatClientImpl::handleRegisterUpdateResponse(
    const RegisterUpdateUserResponseJson& responseJson)
{
    REGISTER_UPDATE_USER_STATUS status = responseJson.getStatus();
    for (IChatClientListener* listener: m_clientListeners)
    {
        listener->onRegisterUpdateResponse(status);
    }
}
