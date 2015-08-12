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

void ChatClientImpl::connect(const std::string& address,
                             uint16_t           port)
{
    p_websocketClient->connect(address, port);
}

void ChatClientImpl::login(const UserCredentials& userCredentials)
{

    if (m_state == CONNECTED ||
        m_state == LOG_IN_ERROR)
    {
        std::string loginJSON = p_jsonFactory->createLoginJsonString(
            userCredentials);
        p_websocketClient->sendMessage(loginJSON);

        m_state = LOGGING_IN;
    }

    // else handle login after connecting to the server
//    if (m_state == INITIAL ||
//        m_state == DISCONNECTED ||
//        m_state == CONNECT_ERROR)
//    {
//        m_state = CONNECTING;

//        if (!p_websocketClient->connect())
//        {
//            m_state = CONNECT_ERROR;
//        }
//    }
}

void ChatClientImpl::sendMessage(int receiverId, const std::string& message)
{
    Message m(message,receiverId);
    std::string sendMessageJson = p_jsonFactory->createSendMessageJsonString(m);
    p_websocketClient->sendMessage(sendMessageJson);
}

void ChatClientImpl::requestContacts()
{
    std::string requestJson = p_jsonFactory->createRequestContactsJsonString();

    p_websocketClient->sendMessage(requestJson);
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
    std::string requestJson =
        p_jsonFactory->createAddContactJsonString(userName);

    p_websocketClient->sendMessage(requestJson);

}

void ChatClientImpl::removeContact(int contactId)
{
    std::string requestJson = p_jsonFactory->createRemoveContactJsonString(
        contactId);

    p_websocketClient->sendMessage(requestJson);

}

void ChatClientImpl::onMessageReceived(const std::string& message)
{
    p_jsonParser->trySetJsonString(message);
    RESPONSE_ACTION_TYPE action = p_jsonParser->getActionType();
//    LoginResponseJson a;
    switch (action)
    {
        case RESPONSE_LOGIN:
        {
//            a = p_jsonParser->tryGetLoginResponseJson();
            handleLoginResponse(p_jsonParser->tryGetLoginResponseJson());
            LOG_DEBUG("after resp log\n");
            break;
        }

        case RESPONSE_GET_CONTACTS:
        {
            handleReceiveContacts(p_jsonParser->tryGetReceiveContactsJson());
            break;
        }

        case RESPONSE_SEND_MESSAGE:
        {
            handleReceiveMessage(p_jsonParser->tryGetReceiveMessageJson());
            break;
        }

        case RESPONSE_CONTACT_STATE_CHANGED:
        {
            handleContactStateChanged(
                p_jsonParser->tryGetContactStateChangedJson());
            break;
        }

        case RESPONSE_ADD_CONTACT:
        {

            const AddingByContactJson& responseJson = p_jsonParser->tryGetAddingByContactJson();
            bool accept = handleAddingByContact(responseJson);

            std::string requestJson = p_jsonFactory->createAddContactResolutionJsonString(
                                            (responseJson.getUserName()) , accept);

            p_websocketClient->sendMessage(requestJson);
            break;
        }
        case RESPONSE_ADD_CONTACT_RESOLUTION:
        {
            handleAddContactResponse(p_jsonParser->tryGetAddContactResponseJson());
            //TODO: send add contact notification to listener and wait for result

            break;
        }

        case RESPONSE_REMOVE_CONTACT:
        {
            handleRemovedByContact(p_jsonParser->tryGetRemovedByContactJson());
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
    m_state = CONNECTED;
    for (auto listener: m_clientListeners)
    {
        listener->onConnected();
    }
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
    m_state = CONNECT_ERROR;
    for (auto listener: m_clientListeners)
    {
        listener->onConnectionError();
    }
}

void ChatClientImpl::handleLoginResponse(const LoginResponseJson& responseJson)
{

    AUTH_STATUS status = responseJson.getAutheticationStatus();
    switch(status)
    {
        case AUTH_SUCCESSFUL:
        {
            m_state = LOGGED_IN;
            for (auto listener: m_clientListeners)
            {
                listener->onLoginSuccessful(
                    responseJson.getUserDetails());
            }
            break;
        }

        case AUTH_INVALID_CREDENTIALS:
        {
            m_state = LOG_IN_ERROR;
            for (auto listener: m_clientListeners)
            {
                listener->onLoginFailed("INVALID CREDENTIALS");
            }
            LOG_DEBUG_METHOD;
            break;
        }

        case AUTH_ALREADY_LOGGED_IN:
        {
            m_state = LOG_IN_ERROR;
            for (IChatClientListener* listener: m_clientListeners)
            {
                LOG_DEBUG("Before\n");
                listener->onLoginFailed("USER ALREADY LOGGED IN");
                LOG_DEBUG("After\n");
            }
            LOG_DEBUG("it fin\n");
            break;
        }
    }
    LOG_DEBUG("meth end\n");
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
    CONTACT_STATE contactState = responseJson.getContactState();
//    LOG_DEBUG("%d %d\n",userId,jsonObject);
    for (auto listener: m_clientListeners)
    {
        listener->onContactStateChanged(contactId, contactState);
    }
}

bool ChatClientImpl::handleAddingByContact(const AddingByContactJson& responseJson)
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

void ChatClientImpl::handleAddContactResponse(const AddContactResponseJson& responseJson)
{
    const std::string& userName = responseJson.getUserName();
    bool accepted = responseJson.hasAccepted();
    for (IChatClientListener* listener: m_clientListeners)
    {
        listener->onAddContactResponse(userName,accepted);
    }
}

void ChatClientImpl::handleRemovedByContact(const RemovedByContactJson& responseJson)
{
    int contactId = responseJson.getContactId();
    for (IChatClientListener* listener: m_clientListeners)
    {
        listener->onRemovedByContact(contactId);
    }
}

