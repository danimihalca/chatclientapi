#include "ChatClient/ChatClientImpl.hpp"
#include "ChatClient/ChatClient.hpp"

ChatClient::ChatClient() :
    p_impl(new ChatClientImpl())
{
}

ChatClient::~ChatClient()
{
    p_impl.release();
}

void ChatClient::setServerProperties(const std::string& address, uint16_t port)
{
    p_impl->setServerProperties(address, port);

}

void ChatClient::login(const std::string& user, const std::string& password)
{
    p_impl->login(user, password);

}

void ChatClient::sendMessage(const std::string& message)
{
    p_impl->sendMessage(message);
}

void ChatClient::addChatClientListener(std::shared_ptr<IChatClientListener>& listener)
{
    p_impl->addChatClientListener(listener);
}

void ChatClient::removeChatClientListener(std::shared_ptr<IChatClientListener>& listener)
{
    p_impl->removeChatClientListener(listener);
}

void ChatClient::disconnect()
{
    p_impl->disconnect();
}
