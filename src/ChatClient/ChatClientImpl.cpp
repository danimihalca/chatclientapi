#include "ChatClient/ChatClientImpl.h"
#include "ChatClient/IChatClientListener.h"
#include "WebsocketClient/WebsocketClient.h"

ChatClientImpl::ChatClientImpl() :
    p_websocketClient(new WebsocketClient()),
    m_clientListeners()
{
}

ChatClientImpl::~ChatClientImpl()
{

}

void ChatClientImpl::connect(const std::string& address, uint16_t port)
{
    p_websocketClient->initialize();
    p_websocketClient->connect(address, port);
    p_websocketClient->startService();
}

void ChatClientImpl::sendMessage(const std::string& message)
{
    p_websocketClient->sendMessage(message);
}

void ChatClientImpl::addChatClientListener(IChatClientListener* listener)
{
    m_clientListeners.push_back(listener);
}

void ChatClientImpl::removeChatClientListener(IChatClientListener* listener)
{
    m_clientListeners.remove(listener);
}

void ChatClientImpl::disconnect()
{
    p_websocketClient->closeConnection();
}

void ChatClientImpl::onMessageReceived(const std::string& message)
{
    for (auto listener: m_clientListeners)
    {
        listener->onMessageReceived(message);
    }
}

void ChatClientImpl::onConnected()
{
    for (auto listener: m_clientListeners)
    {
        listener->onConnected();
    }
}

void ChatClientImpl::onDisconnected()
{
    for (auto listener: m_clientListeners)
    {
        listener->onDisconnected();
    }
}

