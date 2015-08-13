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

void ChatClient::login(const UserCredentials& userCredentials, USER_STATE state)
{
    p_impl->login(userCredentials,state);

}

void ChatClient::changeState(USER_STATE state)
{
    p_impl->changeState(state);
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


void ChatClient::registerUser(const User& user)
{
    p_impl->registerUser(user);
}

void ChatClient::updateUser(const User& user)
{
    p_impl->updateUser(user);
}
