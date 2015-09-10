#ifndef CLIENTJSONPARSER_HPP
#define CLIENTJSONPARSER_HPP

#include <string>

#include <json/json.h>

#include "INotificationParser.hpp"



class JsonNotificationParser : public INotificationParser
{
public:
    JsonNotificationParser();
    ~JsonNotificationParser();

    // Implements INotificationParser interface
public:
    bool trySetNotificationString(const std::string& json);
    NOTIFICATION_TYPE getNotificationType();

    LoginResponseNotification tryGetLoginResponseNotification();
    ContactStateChangedNotification tryGetContactStateChangedNotification();
    ReceiveMessageNotification tryGetReceiveMessageNotification();
    ReceiveContactsNotification tryGetReceiveContactsNotification();

    AddRequestNotification tryGetAddRequestNotification();
    AddContactResponseNotification tryGetAddContactResponseNotification();
    RemovedByContactNotification tryGetRemovedByContactNotification();

    RegisterUpdateNotification tryGetRegisterUpdateUserNotification();

private:
    Json::CharReader* p_reader;
    Json::Value m_root;

};

#endif // CLIENTJSONPARSER_HPP
