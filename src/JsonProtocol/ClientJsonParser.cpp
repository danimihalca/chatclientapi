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

bool ClientJsonParser::trySetJsonString(const std::string& json)
{
    LOG_DEBUG("PARSING : %s\n",json.c_str());
    const char* cString = json.c_str();
    return p_reader->parse(cString,
                           cString + strlen(cString),
                           &m_root,
                           nullptr);
}

RESPONSE_ACTION_TYPE ClientJsonParser::getActionType()
{
    return static_cast<RESPONSE_ACTION_TYPE>(m_root[RESPONSE_ACTION].asInt());
}


LoginResponseJson ClientJsonParser::tryGetLoginResponseJson()
{
    Json::Value content = m_root[CONTENT];

    AUTH_STATUS status =
        static_cast<AUTH_STATUS>(content[AUTHENTICATION_STATUS].asInt());

    UserDetails userDetails;
    userDetails.setId(content[USER_DETAILS][ID].asInt());
    userDetails.setFirstName(content[USER_DETAILS][FIRSTNAME].asString());
    userDetails.setLastName(content[USER_DETAILS][LASTNAME].asString());

    LoginResponseJson responseJson;
    responseJson.setAutheticationStatus(status);
    responseJson.setUserDetails(userDetails);

    return responseJson;
}

ContactStateChangedJson ClientJsonParser::tryGetContactStateChangedJson()
{
    Json::Value content = m_root[CONTENT];

    int contactId = content[CONTACT][ID].asInt();
    CONTACT_STATE state =
        static_cast<CONTACT_STATE>(content[CONTACT][STATE].asInt());
    ContactStateChangedJson responseJson;

    responseJson.setContactId(contactId);
    responseJson.setContactState(state);

    return responseJson;
}

ReceiveMessageJson ClientJsonParser::tryGetReceiveMessageJson()
{
    Json::Value content = m_root[CONTENT];

    Message message(
        content[MESSAGE][SENDER_ID].asInt(),
        content[MESSAGE][MESSAGE_TEXT].asString());

    ReceiveMessageJson responseJson;

    responseJson.setMessage(message);

    return responseJson;
}

ReceiveContactsJson ClientJsonParser::tryGetReceiveContactsJson()
{
    Json::Value content = m_root[CONTENT];

    Json::Value contactsJson = content[CONTACTS];

    std::vector<Contact> contacts(contactsJson.size());

    int count = 0;
    for (auto& contactJson: contactsJson)
    {
        contacts[count++] = Contact(contactJson[ID].asInt(),
                                    contactJson[USERNAME].asString(),
                                    contactJson[FIRSTNAME].asString(),
                                    contactJson[LASTNAME].asString(),
                                    static_cast<CONTACT_STATE>(contactJson[STATE]
                                                               .asInt()));
    }


    ReceiveContactsJson responseJson;
    responseJson.setContacts(contacts);

    return responseJson;

}
