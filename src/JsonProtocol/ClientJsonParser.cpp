#include "JsonProtocol/ClientJsonParser.hpp"

#include <iostream>

#include <cstring>

#include <debug_utils/log_debug.hpp>

#include <Model/User.hpp>

#include <JsonChatProtocol/json_response/LoginResponseJson.hpp>
#include <JsonChatProtocol/json_response/ContactStateChangedJson.hpp>
#include <JsonChatProtocol/json_response/ReceiveMessageJson.hpp>
#include <JsonChatProtocol/json_response/ReceiveContactsJson.hpp>

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

IResponseJson* ClientJsonParser::tryGetResponseJson(RESPONSE_ACTION_TYPE type)
{
    switch (type)
    {
        case RESPONSE_LOGIN:
        {
            return getLoginResponseJson();
        }

//        case CONTACT_UPDATED:
//        {
//            getContactUpdatedResponse();
//        }

        case RESPONSE_CONTACT_STATE_CHANGED:
        {
            return getContactStateChangedJson();
        }

        case RESPONSE_SEND_MESSAGE:
        {
            return getReceiveMessageJson();
        }

        case RESPONSE_GET_CONTACTS:
        {
            return getReceiveContactsJson();
        }
    }
}

//AUTH_STATUS ClientJsonParser::getAuthenticationStatus()
//{
//    return static_cast<AUTHENTICATION_STATUS>(m_root[AUTHENTICATION_STATUS].
//                                              asInt());
//}


//UserDetails ClientJsonParser::getUserDetails()
//{
//    int userId = m_root[USER_DETAILS][USER_ID].asInt();
//    std::string fullName = m_root[USER_DETAILS][USER_FIRSTNAME].asString();
//    return UserDetails(userId, fullName);
//}

//Contacts ClientJsonParser::getContacts()
//{
//    Json::Value contactsJson = m_root[CONTACTS];

//    Contacts contacts(contactsJson.size());

//    int count = 0;
//    for (auto contactJson: contactsJson)
//    {
//        contacts[count++] = Contact(contactJson[USER_ID].asInt(),
//                                    contactJson[USER_USERNAME].asString(),
//                                    contactJson[USER_FIRSTNAME].asString(),
//                                    contactJson[CONTACT_ONLINE].asBool());
//    }

//    return contacts;
//}

//Message ClientJsonParser::getMessage()
//{
//    int senderId = m_root[MESSAGE][MESSAGE_SENDER_ID].asInt();
//    std::string messageText = m_root[MESSAGE][MESSAGE_TEXT].asString();

//    return Message(senderId,messageText);
//}

//int ClientJsonParser::getUserId()
//{
//    return m_root[USER_ID].asInt();
//}

IResponseJson* ClientJsonParser::getLoginResponseJson()
{
    Json::Value content = m_root[CONTENT];

    AUTH_STATUS status =
        static_cast<AUTH_STATUS>(content[AUTHENTICATION_STATUS].asInt());

    UserDetails userDetails;
    userDetails.setId(content[USER_DETAILS][ID].asInt());
    userDetails.setFirstName(content[USER_DETAILS][FIRSTNAME].asString());
    userDetails.setLastName(content[USER_DETAILS][LASTNAME].asString());

    LoginResponseJson* responseJson = new LoginResponseJson();
    responseJson->setAutheticationStatus(status);
    responseJson->setUserDetails(userDetails);

    return responseJson;
}

IResponseJson* ClientJsonParser::getContactStateChangedJson()
{
    Json::Value content = m_root[CONTENT];

    int contactId = content[CONTACT][ID].asInt();
    CONTACT_STATE state =
        static_cast<CONTACT_STATE>(content[CONTACT][STATE].asInt());
    ContactStateChangedJson* responseJson = new ContactStateChangedJson();

    responseJson->setContactId(contactId);
    responseJson->setContactState(state);

    return responseJson;
}

IResponseJson* ClientJsonParser::getReceiveMessageJson()
{
    Json::Value content = m_root[CONTENT];

    Message message(
        content[MESSAGE][SENDER_ID].asInt(),
        content[MESSAGE][MESSAGE_TEXT].asString());

    ReceiveMessageJson* responseJson = new ReceiveMessageJson();

    responseJson->setMessage(message);

    return responseJson;
}

IResponseJson* ClientJsonParser::getReceiveContactsJson()
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


    ReceiveContactsJson* responseJson = new ReceiveContactsJson();
    responseJson->setContacts(contacts);

    return responseJson;

}
