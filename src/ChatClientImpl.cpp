#include "ChatClientImpl.h"

#ifdef ANDROID
#include "android/log.h"
#endif

void ChatClientImpl::initialize()
{
}

void ChatClientImpl::setNewMessageCallback(newMessageCallback callback)
{
    m_newMessageCB = callback;
}

void ChatClientImpl::connect(const std::string& uri)
{

}

void ChatClientImpl::startService()
{
}



void ChatClientImpl::sendMessage(const std::string& message)
{

}

void ChatClientImpl::closeConnection()
{

}

ChatClientImpl::~ChatClientImpl()
{
    if (m_connected)
    {
        closeConnection();
    }
//    if (p_serviceThread->joinable())
//    {
//        p_serviceThread->join();
//    }
}




