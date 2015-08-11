#ifndef ICLIENTJSONPARSER_HPP
#define ICLIENTJSONPARSER_HPP

#include <string>

#include <JsonChatProtocol/common_json_protocol.hpp>

#include <Model/User.hpp>
#include <Model/Message.hpp>

#include <JsonChatProtocol/json_response/LoginResponseJson.hpp>
#include <JsonChatProtocol/json_response/ContactStateChangedJson.hpp>
#include <JsonChatProtocol/json_response/ReceiveMessageJson.hpp>
#include <JsonChatProtocol/json_response/ReceiveContactsJson.hpp>


class IClientJsonParser
{
public:
    virtual bool trySetJsonString(const std::string& json) = 0;
    virtual RESPONSE_ACTION_TYPE getActionType() = 0;

    virtual LoginResponseJson tryGetLoginResponseJson() = 0;
    virtual ContactStateChangedJson tryGetContactStateChangedJson() = 0;
    virtual ReceiveMessageJson tryGetReceiveMessageJson() = 0;
    virtual ReceiveContactsJson tryGetReceiveContactsJson() = 0;

    virtual ~IClientJsonParser()
    {
    }
};

#endif // ICLIENTJSONPARSER_HPP

