#include "ChatClient/ChatClientImpl.h"
#include "ChatClient/ChatClient.h"

ChatClient::ChatClient() :
    p_impl(new ChatClientImpl())
{
}

void ChatClient::connect(const std::string& address, uint16_t port)
{
    p_impl->connect(address, port);
}

void ChatClient::sendMessage(const std::string& message)
{
    p_impl->sendMessage(message);
}

void ChatClient::addChatClientListener(IChatClientListener* listener)
{
    p_impl->addChatClientListener(listener);
}

void ChatClient::removeChatClientListener(IChatClientListener* listener)
{
    p_impl->removeChatClientListener(listener);
}

void ChatClient::disconnect()
{
    p_impl->disconnect();
}
