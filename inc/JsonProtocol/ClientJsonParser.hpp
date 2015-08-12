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

    LoginResponseJson tryGetLoginResponseJson();
    ContactStateChangedJson tryGetContactStateChangedJson();
    ReceiveMessageJson tryGetReceiveMessageJson();
    ReceiveContactsJson tryGetReceiveContactsJson();

    AddingByContactJson tryGetAddingByContactJson();
    AddContactResponseJson tryGetAddContactResponseJson();
    RemovedByContactJson tryGetRemovedByContactJson();

private:
    Json::CharReader* p_reader;
    Json::Value m_root;

};

#endif // CLIENTJSONPARSER_HPP
