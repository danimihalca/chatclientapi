#include "ChatClient/ChatClientImpl.hpp"
#include "ChatClient/IChatClientListener.hpp"

#include "WebsocketClient/WebsocketClient.hpp"

#include "JsonProtocol/ClientJsonFactory.hpp"
#include "JsonProtocol/ClientJsonParser.hpp"

#include <debug_utils/log_debug.hpp>

#include <Model/Message.hpp>

ChatClientImpl::ChatClientImpl() :
    m_user(),
    m_state(INITIAL),
    p_websocketClient(new WebsocketClient()),
    m_clientListeners(),
    p_jsonFactory(new ClientJsonFactory()),
    p_jsonParser(new ClientJsonParser())
{
    p_websocketClient->addWebsocketClientListener(this);
}

ChatClientImpl::~ChatClientImpl()
{
    p_websocketClient.reset();
}

void ChatClientImpl::setServerProperties(const std::string& address,
                                         uint16_t           port)
{
    p_websocketClient->setServerProperties(address,port);
}

void ChatClientImpl::login(const std::string& userName,
                           const std::string& password)
{
    UserCredentials userCredentials(userName,password);
    m_user.setUserCredentials(userCredentials);

    if (m_state == CONNECTED ||
        m_state == LOG_IN_ERROR)
    {
        performLogin();
    }

    // else handle login after connecting to the server
    if (m_state == INITIAL ||
        m_state == DISCONNECTED ||
        m_state == CONNECT_ERROR)
    {
        m_state = CONNECTING;

        if (!p_websocketClient->connect())
        {
            m_state = CONNECT_ERROR;
        }
    }
}

void ChatClientImpl::sendMessage(int receiverId, const std::string& message)
{
    Message m(message,receiverId);
    std::string sendMessageJson = p_jsonFactory->createSendMessageJsonString(m);
    p_websocketClient->sendMessage(sendMessageJson);
}

void ChatClientImpl::getContacts()
{
    std::string requestJson = p_jsonFactory->createGetContactsRequestJsonString(
        m_user);

    p_websocketClient->sendMessage(requestJson);
}

void ChatClientImpl::addChatClientListener(
    std::shared_ptr<IChatClientListener>& listener)
{
    m_clientListeners.push_back(listener);
}

void ChatClientImpl::removeChatClientListener(
    std::shared_ptr<IChatClientListener>& listener)
{
    m_clientListeners.remove(listener);
}

void ChatClientImpl::disconnect()
{
    p_websocketClient->closeConnection();
}

void ChatClientImpl::onMessageReceived(const std::string& message)
{
    p_jsonParser->parseJsonString(message);
    Chat_Action_Type action = p_jsonParser->getActionType();
    switch (action)
    {
        case LOGIN_RESPONSE:
        {
            handleLoginResponse();
            break;
        }

        case GET_CONTACTS_RESPONSE:
        {
            handleGetContactsResponse();
            break;
        }

        case RECEIVE_MESSAGE:
        {
            Message message = p_jsonParser->getMessage();
            LOG_DEBUG("M:%s\n",message.getMessageText().c_str());
            for (auto listener: m_clientListeners)
            {
                listener->onMessageReceived(message.getMessageText());
            }
            break;
        }

        default:
        {
            break;
        }
    }
}

void ChatClientImpl::onConnected()
{
    m_state = CONNECTED;
    for (auto listener: m_clientListeners)
    {
        listener->onConnected();
    }

    performLogin();
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

void ChatClientImpl::performLogin()
{
    std::string loginJSON = p_jsonFactory->createLoginJsonString(
        m_user.getUserCredentials());
    p_websocketClient->sendMessage(loginJSON);

    m_state = LOGGING_IN;
}

void ChatClientImpl::handleLoginResponse()
{
    Authentication_Status status =
        p_jsonParser->getAuthenticationStatus();
    switch(status)
    {
        case AUTH_SUCCESSFUL:
        {
            m_state = LOGGED_IN;
            m_user.setUserDetails(p_jsonParser->getUserDetails());
            for (auto listener: m_clientListeners)
            {
                listener->onLoginSuccessful();
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

void ChatClientImpl::handleGetContactsResponse()
{
    Contacts contacts = p_jsonParser->getContacts();
    for(Contact contact: contacts)
    {
        LOG_DEBUG("Contact: %s\n", contact.getUserName().c_str());
    }

    for (auto listener: m_clientListeners)
    {
        listener->onContactsReceived(contacts);
    }
}
