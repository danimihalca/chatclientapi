#ifndef ICLIENTJSONPARSER_HPP
#define ICLIENTJSONPARSER_HPP

#include <string>

#include <JsonChatProtocol/common_json_protocol.hpp>

#include <Model/User.hpp>
#include <Model/Message.hpp>

class IResponseJson;

class IClientJsonParser
{
public:
    virtual bool trySetJsonString(const std::string& json) = 0;
    virtual RESPONSE_ACTION_TYPE getActionType() = 0;
    virtual IResponseJson* tryGetResponseJson(RESPONSE_ACTION_TYPE type) = 0;


    virtual ~IClientJsonParser()
    {
    }
};

#endif // ICLIENTJSONPARSER_HPP

