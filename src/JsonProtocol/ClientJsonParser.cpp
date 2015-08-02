#include "JsonProtocol/ClientJsonParser.hpp"

#include <iostream>

#include <cstring>

#include <debug_utils/log_debug.hpp>

#include <Model/User.hpp>

ClientJsonParser::ClientJsonParser()
{
    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    builder["allowComments"] = true;
    p_reader = builder.newCharReader();
}

ClientJsonParser::~ClientJsonParser()
{
    delete p_reader;
}

bool ClientJsonParser::parseJsonString(const std::string& json)
{
    log_debug("PARSING : %s\n",json.c_str());
    const char* cString = json.c_str();
    return p_reader->parse(cString,
                           cString + strlen(cString),
                           &m_root,
                           nullptr);
}

Chat_Action_Type ClientJsonParser::getActionType()
{
    return static_cast<Chat_Action_Type>(m_root[ACTION].asInt());
}

Authentication_Status ClientJsonParser::getAuthenticationStatus()
{
    return static_cast<Authentication_Status>(m_root[AUTHENTICATION_STATUS].
                                              asInt());
}


UserDetails ClientJsonParser::getUserDetails()
{
    int userId = m_root[USER_DETAILS][USER_ID].asInt();
    std::string fullName =  m_root[USER_DETAILS][USER_FULLNAME].asString();
    return UserDetails(userId, fullName);
}
