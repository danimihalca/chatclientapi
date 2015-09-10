#ifndef ICHATCLIENTLISTENER_HPP
#define ICHATCLIENTLISTENER_HPP

#include <string>

#include <Model/User.hpp>
#include <Model/Message.hpp>

#include <JsonChatProtocol/common_json_protocol.hpp>

class IChatClientListener
{
public:
    virtual void onMessageReceived(const Message& message) = 0;
    virtual void onLoginSuccessful(const UserDetails& userDetails) = 0;
    virtual void onLoginFailed(AUTH_STATUS reason) = 0;
    virtual void onConnectionError() = 0;
    virtual void onDisconnected() = 0;
    virtual void onContactsReceived(const std::vector<Contact>& contacts) = 0;
    virtual void onContactStateChanged(int contactId, USER_STATE state) = 0;


    virtual void onRemovedByContact(int contactId) = 0;
    virtual void onAddContactResponse(const std::string& userName,
                                      ADD_STATUS         status) = 0;
    virtual void onRegisterUpdateResponse(REGISTER_UPDATE_USER_STATUS status) =
        0;

    virtual bool onAddRequest(const std::string& requester) = 0;
    virtual ~IChatClientListener()
    {
    }
};

#endif // ICHATCLIENTLISTENER_HPP

