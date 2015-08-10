#ifndef ICHATCLIENTLISTENER_HPP
#define ICHATCLIENTLISTENER_HPP

#include <string>

#include <Model/User.hpp>
#include <Model/Message.hpp>

class IChatClientListener
{
public:
    virtual void onMessageReceived(const Message& message) = 0;
    virtual void onConnected() = 0;
    virtual void onLoginSuccessful(const UserDetails& userDetails) = 0;
    virtual void onLoginFailed(const std::string& message) = 0;
    virtual void onConnectionError() = 0;
    virtual void onDisconnected() = 0;
    virtual void onContactsReceived(const std::vector<Contact>& contacts) = 0;
    virtual void onContactStateChanged(int contactId, CONTACT_STATE state) = 0;
    virtual ~IChatClientListener()
    {
    }
};

#endif // ICHATCLIENTLISTENER_HPP

