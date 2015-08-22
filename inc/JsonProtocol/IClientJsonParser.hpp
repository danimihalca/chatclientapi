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

#include <JsonChatProtocol/json_response/AddingByContactJson.hpp>
#include <JsonChatProtocol/json_response/AddContactResponseJson.hpp>
#include <JsonChatProtocol/json_response/RemovedByContactJson.hpp>
#include <JsonChatProtocol/json_response/RegisterUpdateUserResponseJson.hpp>

class IClientJsonParser
{
public:
    virtual bool trySetJsonString(const std::string& json) = 0;
    virtual NOTIFICATION_TYPE getActionType() = 0;

    virtual LoginResponseJson tryGetLoginResponseJson() = 0;
    virtual ContactStateChangedJson tryGetContactStateChangedJson() = 0;
    virtual ReceiveMessageJson tryGetReceiveMessageJson() = 0;
    virtual ReceiveContactsJson tryGetReceiveContactsJson() = 0;

    virtual AddingByContactJson tryGetAddingByContactJson() = 0;
    virtual AddContactResponseJson tryGetAddContactResponseJson() = 0;
    virtual RemovedByContactJson tryGetRemovedByContactJson() = 0;

    virtual RegisterUpdateUserResponseJson tryGetRegisterUpdateUserJson() = 0;

    virtual ~IClientJsonParser()
    {
    }
};

#endif // ICLIENTJSONPARSER_HPP

