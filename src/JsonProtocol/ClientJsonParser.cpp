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
    LOG_DEBUG("PARSING : %s\n",json.c_str());
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
    std::string fullName = m_root[USER_DETAILS][USER_FULLNAME].asString();
    return UserDetails(userId, fullName);
}

Contacts ClientJsonParser::getContacts()
{
    Json::Value contactsJson = m_root[CONTACTS];

    Contacts contacts(contactsJson.size());

    int count = 0;
    for (auto contactJson: contactsJson)
    {
        contacts[count++] = Contact(contactJson[USER_ID].asInt(),
                                    contactJson[USER_USERNAME].asString(),
                                    contactJson[USER_FULLNAME].asString(),
                                    contactJson[CONTACT_ONLINE].asBool());
    }

    return contacts;
}

Message ClientJsonParser::getMessage()
{
    int senderId = m_root[MESSAGE][MESSAGE_SENDER_ID].asInt();
    std::string messageText = m_root[MESSAGE][MESSAGE_TEXT].asString();

    return Message(senderId,messageText);
}

int ClientJsonParser::getUserId()
{
    return m_root[USER_ID].asInt();
}
