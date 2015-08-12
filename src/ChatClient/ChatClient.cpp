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

void ChatClient::connect(const std::string& address, uint16_t port)
{
    p_impl->connect(address, port);

}

void ChatClient::login(const UserCredentials& userCredentials)
{
    p_impl->login(userCredentials);

}

void ChatClient::sendMessage(int receiverId, const std::string& message)
{
    p_impl->sendMessage(receiverId,message);
}

void ChatClient::requestContacts()
{
    p_impl->requestContacts();
}

void ChatClient::addListener(IChatClientListener* listener)
{
    p_impl->addListener(listener);
}

void ChatClient::removeListener(IChatClientListener* listener)
{
    p_impl->removeListener(listener);
}

void ChatClient::disconnect()
{
    p_impl->disconnect();
}

void ChatClient::addContact(const std::string& userName)
{
    p_impl->addContact(userName);
}

void ChatClient::removeContact(int contactId)
{
    p_impl->removeContact(contactId);
}
