#include "ChatClient/ChatClientImpl.h"
#include "ChatClient/ChatClient.h"

ChatClient::ChatClient(const std::string& address, uint16_t port) :
    p_impl(new ChatClientImpl(address, port))
{
}

ChatClient::~ChatClient()
{
  p_impl.release();
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
