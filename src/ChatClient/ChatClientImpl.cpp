#include "ChatClient/ChatClientImpl.h"
#include "ChatClient/IChatClientListener.h"
#include "WebsocketClient/WebsocketClient.h"

#include "JsonProtocol/JsonFactory.h"
#include "JsonProtocol/JsonParser.h"

ChatClientImpl::ChatClientImpl() :
    m_connectionStatus(NOT_CONNECTED),
    p_websocketClient(new WebsocketClient()),
    m_clientListeners(),
    p_jsonFactory(new JsonFactory()),
    p_jsonParser(new JsonParser())
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

void ChatClientImpl::login(const std::string& user, const std::string& password)
{
    if (m_connectionStatus != CONNECTED)
    {
        m_connectionStatus = NOT_CONNECTED;
        bool isConnecting = p_websocketClient->connect();
        if (!isConnecting)
        {
            return;
        }
    }
    while (m_connectionStatus == NOT_CONNECTED)
    {
    }
    if (m_connectionStatus == CONNECTION_ERROR)
    {
        return;
    }
    std::string loginJSON = p_jsonFactory->createLoginJSON(user,password);
    p_websocketClient->sendMessage(loginJSON);
}

void ChatClientImpl::sendMessage(const std::string& message)
{
    p_websocketClient->sendMessage(message);
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
    ChatServer_Action_Type action = p_jsonParser->getActionType();
    switch (action)
    {
        case CS_LOGIN_RESPONSE:
        {
            Authentification_Status status =
                p_jsonParser->getAuthentificationStatus();
            switch(status)
            {
                case AUTH_OK:
                {
                    for (auto listener: m_clientListeners)
                    {
                        listener->onLoginSuccessfull();
                    }
                    break;
                }

                case AUTH_FAILED:
                {
                    for (auto listener: m_clientListeners)
                    {
                        listener->onLoginFailed("INVALID CREDENTIALS");
                    }
                    break;
                }

                case AUTH_ALREADY_LOGGED_IN:
                {
                    for (auto listener: m_clientListeners)
                    {
                        listener->onLoginFailed("USER ALREADY LOGGED IN");
                    }
                    break;
                }
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
    m_connectionStatus = CONNECTED;
    for (auto listener: m_clientListeners)
    {
        listener->onConnected();
    }
}

void ChatClientImpl::onDisconnected()
{
    m_connectionStatus = NOT_CONNECTED;
    for (auto listener: m_clientListeners)
    {
        listener->onDisconnected();
    }
}

void ChatClientImpl::onConnectionError()
{
    for (auto listener: m_clientListeners)
    {
        listener->onConnectionError();
    }
    m_connectionStatus = CONNECTION_ERROR;
}

