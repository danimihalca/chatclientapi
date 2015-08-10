#ifndef CLIENTJSONPARSER_HPP
#define CLIENTJSONPARSER_HPP

#include <string>

#include <json/json.h>

#include "IClientJsonParser.hpp"

class ClientJsonParser: public IClientJsonParser
{
public:
    ClientJsonParser();
    ~ClientJsonParser();

    // Implements IJsonParser interface
public:
    bool trySetJsonString(const std::string& json);
    RESPONSE_ACTION_TYPE getActionType();
    IResponseJson* tryGetResponseJson(RESPONSE_ACTION_TYPE type);


private:
    IResponseJson* getLoginResponseJson();
    IResponseJson* getContactStateChangedJson();
    IResponseJson* getReceiveMessageJson();
    IResponseJson* getReceiveContactsJson();

private:
    Json::CharReader* p_reader;
    Json::Value m_root;


    // IClientJsonParser interface
public:
};

#endif // CLIENTJSONPARSER_HPP
