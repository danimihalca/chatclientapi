#include "protocol/JsonNotificationParser.hpp"

#include <iostream>

#include <cstring>

#include <debug_utils/log_debug.hpp>

#include <Model/User.hpp>



JsonNotificationParser::JsonNotificationParser()
{
    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    builder["allowComments"] = true;
    p_reader = builder.newCharReader();
}

JsonNotificationParser::~JsonNotificationParser()
{
    delete p_reader;
}

bool JsonNotificationParser::trySetNotificationString(const std::string& json)
{
    LOG_DEBUG("PARSING : %s\n",json.c_str());
    const char* cString = json.c_str();
    return p_reader->parse(cString,
                           cString + strlen(cString),
                           &m_root,
                           nullptr);
}

NOTIFICATION_TYPE JsonNotificationParser::getNotificationType()
{
    return static_cast<NOTIFICATION_TYPE>(m_root[NOTIFICATION].asInt());
}


LoginResponseNotification JsonNotificationParser::tryGetLoginResponseNotification()
{
    Json::Value content = m_root[CONTENT];

    AUTH_STATUS status =
        static_cast<AUTH_STATUS>(content[AUTHENTICATION_STATUS].asInt());

    UserDetails userDetails;
    if (status == AUTH_SUCCESSFUL)
    {
        userDetails.setId(content[USER_DETAILS][ID].asInt());
        userDetails.setFirstName(content[USER_DETAILS][FIRSTNAME].asString());
        userDetails.setLastName(content[USER_DETAILS][LASTNAME].asString());
    }

    LoginResponseNotification responseJson;
    responseJson.setAutheticationStatus(status);
    responseJson.setUserDetails(userDetails);

    return responseJson;
}

ContactStateChangedNotification JsonNotificationParser::tryGetContactStateChangedNotification()
{
    Json::Value content = m_root[CONTENT];

    int contactId = content[CONTACT][ID].asInt();
    USER_STATE state =
        static_cast<USER_STATE>(content[CONTACT][STATE].asInt());
    ContactStateChangedNotification responseJson;

    responseJson.setContactId(contactId);
    responseJson.setContactState(state);

    return responseJson;
}

ReceiveMessageNotification JsonNotificationParser::tryGetReceiveMessageNotification()
{
    Json::Value content = m_root[CONTENT];

    Message message(
        content[MESSAGE][SENDER_ID].asInt(),
        content[MESSAGE][MESSAGE_TEXT].asString());

    ReceiveMessageNotification responseJson;

    responseJson.setMessage(message);

    return responseJson;
}

ReceiveContactsNotification JsonNotificationParser::tryGetReceiveContactsNotification()
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
                                    static_cast<USER_STATE>(contactJson[STATE]
                                                            .asInt()));
    }


    ReceiveContactsNotification responseJson;
    responseJson.setContacts(contacts);

    return responseJson;

}

AddRequestNotification JsonNotificationParser::tryGetAddingByContactNotification()
{
    Json::Value content = m_root[CONTENT];

    std::string userName = content[USERNAME].asString();

    AddRequestNotification requestJson(userName);

    return requestJson;
}

AddContactResponseNotification JsonNotificationParser::tryGetAddContactResponseNotification()
{
    Json::Value content = m_root[CONTENT];

    std::string userName = content[USERNAME].asString();
    ADD_STATUS addStatus = static_cast<ADD_STATUS>(content[ADD_REQUEST_STATUS].asInt());
    AddContactResponseNotification requestJson(userName, addStatus);

    return requestJson;
}

RemovedByContactNotification JsonNotificationParser::tryGetRemovedByContactNotification()
{
    Json::Value content = m_root[CONTENT];

    int contactId = content[ID].asInt();

    RemovedByContactNotification requestJson(contactId);

    return requestJson;
}


RegisterUpdateNotification JsonNotificationParser::tryGetRegisterUpdateUserNotification()
{
    Json::Value content = m_root[CONTENT];

    REGISTER_UPDATE_USER_STATUS status =
        static_cast<REGISTER_UPDATE_USER_STATUS>(content[REGISTER_UPDATE_STATUS]
                                                 .
                                                 asInt());

    RegisterUpdateNotification requestJson(status);

    return requestJson;
}
