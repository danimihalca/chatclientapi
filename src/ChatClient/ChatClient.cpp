#include "ChatClient/ChatClientImpl.hpp"
#include "ChatClient/ChatClient.hpp"

#include <debug_utils/log_debug.hpp>

ChatClient::ChatClient() :
    p_impl(new ChatClientImpl())
{
    LOG_DEBUG_METHOD;
}

ChatClient::~ChatClient()
{
    LOG_DEBUG_METHOD;
    p_impl.release();
}

void ChatClient::setServer(const std::string& address, uint16_t port)
{
    LOG_DEBUG_METHOD;
    p_impl->setServer(address, port);
}

void ChatClient::login(const UserCredentials& userCredentials, USER_STATE state)
{
    LOG_DEBUG_METHOD;
    p_impl->login(userCredentials,state);
}

void ChatClient::changeState(USER_STATE state)
{
    LOG_DEBUG_METHOD;
    p_impl->changeState(state);
}

void ChatClient::sendMessage(int receiverId, const std::string& message)
{
    LOG_DEBUG_METHOD;
    p_impl->sendMessage(receiverId,message);
}

void ChatClient::requestContacts()
{
    LOG_DEBUG_METHOD;
    p_impl->requestContacts();
}

void ChatClient::addListener(IChatClientListener* listener)
{
    LOG_DEBUG_METHOD;
    p_impl->addListener(listener);
}

void ChatClient::removeListener(IChatClientListener* listener)
{
    LOG_DEBUG_METHOD;
    p_impl->removeListener(listener);
}

void ChatClient::disconnect()
{
    LOG_DEBUG_METHOD;
    p_impl->disconnect();
}

void ChatClient::addContact(const std::string& userName)
{
    LOG_DEBUG_METHOD;
    p_impl->addContact(userName);
}

void ChatClient::removeContact(int contactId)
{
    LOG_DEBUG_METHOD;
    p_impl->removeContact(contactId);
}


void ChatClient::registerUser(const User& user)
{
    LOG_DEBUG_METHOD;
    p_impl->registerUser(user);
}

void ChatClient::updateUser(const User& user)
{
    LOG_DEBUG_METHOD;
    p_impl->updateUser(user);
}
