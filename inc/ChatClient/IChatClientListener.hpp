#ifndef ICHATCLIENTLISTENER_HPP
#define ICHATCLIENTLISTENER_HPP

#include <string>

#include <Model/User.hpp>

class IChatClientListener
{
public:
    virtual void onMessageReceived(int senderId, const std::string& message) = 0;
    virtual void onConnected() = 0;
    virtual void onLoginSuccessful() = 0;
    virtual void onLoginFailed(const std::string& message) = 0;
    virtual void onConnectionError() = 0;
    virtual void onDisconnected() = 0;
    virtual void onContactsReceived(const Contacts& contacts) = 0;
    virtual ~IChatClientListener()
    {
    }
};

#endif // ICHATCLIENTLISTENER_HPP
