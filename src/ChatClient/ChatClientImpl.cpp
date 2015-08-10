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

void ChatClientImpl::getContacts()
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

void ChatClientImpl::onMessageReceived(const std::string& message)
{
    p_jsonParser->trySetJsonString(message);
    RESPONSE_ACTION_TYPE action = p_jsonParser->getActionType();
    IResponseJson* responseJson = p_jsonParser->tryGetResponseJson(action);
    switch (action)
    {
        case RESPONSE_LOGIN:
        {
            LoginResponseJson* loginResponseJson =
                static_cast<LoginResponseJson*>(responseJson);
            handleLoginResponse(loginResponseJson);
            break;
        }

        case RESPONSE_GET_CONTACTS:
        {
            ReceiveContactsJson* receiveContactsJson =
                static_cast<ReceiveContactsJson*>(responseJson);
            handleReceiveContacts(receiveContactsJson);
            break;
        }

        case RESPONSE_SEND_MESSAGE:
        {
            ReceiveMessageJson* receiveMessageJson =
                static_cast<ReceiveMessageJson*>(responseJson);
            handleReceiveMessage(receiveMessageJson);
            break;
        }

        case RESPONSE_CONTACT_STATE_CHANGED:
        {
            ContactStateChangedJson* contactStateChangedJson =
                static_cast<ContactStateChangedJson*>(responseJson);
            handleContactStateChanged(contactStateChangedJson);
            break;
        }


        default:
        {
            break;
        }
    }
    delete responseJson;
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

void ChatClientImpl::handleLoginResponse(LoginResponseJson* responseJson)
{

    AUTH_STATUS status = responseJson->getAutheticationStatus();
    switch(status)
    {
        case AUTH_SUCCESSFUL:
        {
            m_state = LOGGED_IN;
            for (auto listener: m_clientListeners)
            {
                listener->onLoginSuccessful(
                    responseJson->getUserDetails());
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
            break;
        }

        case AUTH_ALREADY_LOGGED_IN:
        {
            m_state = LOG_IN_ERROR;
            for (auto listener: m_clientListeners)
            {
                listener->onLoginFailed("USER ALREADY LOGGED IN");
            }
            break;
        }
    }
}

void ChatClientImpl::handleReceiveContacts(ReceiveContactsJson* responseJson)
{
    const std::vector<Contact>& contacts = responseJson->getContacts();
//    for(Contact contact: contacts)
//    {
//        LOG_DEBUG("Contact: %s\n", contact.getUserName().c_str());
//    }

    for (auto listener: m_clientListeners)
    {
        listener->onContactsReceived(contacts);
    }
}

void ChatClientImpl::handleReceiveMessage(ReceiveMessageJson* responseJson)
{
    Message message = responseJson->getMessage();
    LOG_DEBUG("M:%s\n",message.getMessageText().c_str());
    for (auto listener: m_clientListeners)
    {
        listener->onMessageReceived(message);
    }
}

void ChatClientImpl::handleContactStateChanged(ContactStateChangedJson* responseJson)
{
    int contactId = responseJson->getContactId();
    CONTACT_STATE contactState = responseJson->getContactState();
//    LOG_DEBUG("%d %d\n",userId,jsonObject);
    for (auto listener: m_clientListeners)
    {
        listener->onContactStateChanged(contactId, contactState);
    }
}
