#ifndef ICLIENTJSONPARSER_HPP
#define ICLIENTJSONPARSER_HPP

#include <string>

#include <JsonChatProtocol/common_json_protocol.hpp>

#include <Model/User.hpp>
#include <Model/Message.hpp>

class IClientJsonParser
{
public:
    virtual bool parseJsonString(const std::string& json) = 0;
    virtual Chat_Action_Type getActionType() = 0;
    virtual Authentication_Status getAuthenticationStatus() = 0;
    virtual UserDetails getUserDetails() = 0;
    virtual Contacts getContacts() = 0;
    virtual Message getMessage() = 0;

    virtual ~IClientJsonParser()
    {
    }
};

#endif // ICLIENTJSONPARSER_HPP

