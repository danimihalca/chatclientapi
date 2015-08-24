#ifndef ICLIENTJSONPARSER_HPP
#define ICLIENTJSONPARSER_HPP

#include <string>

#include <JsonChatProtocol/common_json_protocol.hpp>

#include <Model/User.hpp>
#include <Model/Message.hpp>

#include "NotificationObject/LoginResponseNotification.hpp"
#include "NotificationObject/ContactStateChangedNotification.hpp"
#include "NotificationObject/ReceiveMessageNotification.hpp"
#include "NotificationObject/ReceiveContactsNotification.hpp"

#include "NotificationObject/AddRequestNotification.hpp"
#include "NotificationObject/AddContactResponseNotification.hpp"
#include "NotificationObject/RemovedByContactNotification.hpp"
#include "NotificationObject/RegisterUpdateNotification.hpp"

class INotificationParser
{
public:
    virtual bool trySetNotificationString(const std::string& json) = 0;
    virtual NOTIFICATION_TYPE getNotificationType() = 0;

    virtual LoginResponseNotification tryGetLoginResponseNotification() = 0;
    virtual ContactStateChangedNotification tryGetContactStateChangedNotification() = 0;
    virtual ReceiveMessageNotification tryGetReceiveMessageNotification() = 0;
    virtual ReceiveContactsNotification tryGetReceiveContactsNotification() = 0;

    virtual AddRequestNotification tryGetAddRequestNotification() = 0;
    virtual AddContactResponseNotification tryGetAddContactResponseNotification() = 0;
    virtual RemovedByContactNotification tryGetRemovedByContactNotification() = 0;

    virtual RegisterUpdateNotification tryGetRegisterUpdateUserNotification() = 0;

    virtual ~INotificationParser()
    {
    }
};

#endif // ICLIENTJSONPARSER_HPP

