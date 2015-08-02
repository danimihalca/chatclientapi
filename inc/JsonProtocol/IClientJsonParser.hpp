#ifndef ICLIENTJSONPARSER_HPP
#define ICLIENTJSONPARSER_HPP

#include <string>

#include <JsonChatProtocol/common_json_protocol.hpp>

#include <Model/User.hpp>

class IClientJsonParser
{
public:
    virtual ~IClientJsonParser()
    {
    }

    virtual bool parseJsonString(const std::string& json) = 0 ;
    virtual Chat_Action_Type getActionType() = 0;
    virtual Authentication_Status getAuthenticationStatus() = 0;
    virtual UserDetails getUserDetails() = 0;
};

#endif // ICLIENTJSONPARSER_HPP

