#include "JsonProtocol/JsonParser.h"

#include <iostream>

#include <cstring>

#include <debug_utils/log_debug.h>

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
    log_debug("PARSING : %s\n",json.c_str());
    const char* cString = json.c_str();
    bool successfful = p_reader->parse(cString,
                                       cString + strlen(cString),
                                       &m_root,
                                       nullptr);

    if (!successfful)
    {
        throw Json::Exception("Invalid json");
    }
}

ChatServer_Action_Type JsonParser::getActionType()
{
    return static_cast<ChatServer_Action_Type>(m_root[ACTION].asInt());
}

Authentification_Status JsonParser::getAuthentificationStatus()
{
    return static_cast<Authentification_Status>(m_root[AUTHENTIFICATION_STATUS].
                                                asInt());
}
