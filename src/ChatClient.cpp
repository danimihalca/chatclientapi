#include "ChatClient.h"
#include "ChatClientImpl.h"

ChatClient::ChatClient() : p_impl(new ChatClientImpl())
{
}

void ChatClient::initialize()
{
    p_impl->initialize();
}

void ChatClient::setNewMessageCallback(newMessageCallback callback)
{
    p_impl->setNewMessageCallback(callback);
}

void ChatClient::connect(const std::string& address, uint16_t port)
{
    p_impl->connect(address, port);
}

void ChatClient::startService()
{
    p_impl->startService();
}


void ChatClient::sendMessage(const std::string& message)
{
    p_impl->sendMessage(message);
}

void ChatClient::closeConnection()
{
    p_impl->closeConnection();
}

