#include "JsonProtocol/JsonParser.h"

#include <iostream>

#include <cstring>

JsonParser::JsonParser()
{
    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    builder["allowComments"] = true;
    p_reader = builder.newCharReader();
}

JsonParser::~JsonParser()
{
    delete p_reader;
}

void JsonParser::parseJsonString(const std::string& json)
{
    const char* cString = json.c_str();
    bool successfful = p_reader->parse(cString,
                                       &(cString[strlen(cString)]),&m_root,
                                       &m_errors);

    if (!successfful)
    {
        throw Json::Exception(m_errors);
    }
}

ChatServer_Action_Type JsonParser::getActionType()
{
    return static_cast<ChatServer_Action_Type>(m_root[ACTION].asInt());
}

Authentification_Status JsonParser::getAuthentificationStatus()
{
    return static_cast<Authentification_Status>(m_root[AUTHENTIFICATION_STATUS].asInt());
}
