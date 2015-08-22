#ifndef ICLIENTJSONFACTORY_HPP
#define ICLIENTJSONFACTORY_HPP

#include <string>

#include <Model/User.hpp>

class IActionJsonObject;
class Message;

class IClientJsonFactory
{
public:
    virtual IActionJsonObject* createLoginJson(
        const UserCredentials& userCredentials,  USER_STATE state) = 0;
    virtual IActionJsonObject* createRequestContactsJson() = 0;
    virtual IActionJsonObject* createSendMessageJson(const Message& message) =
        0;

    virtual IActionJsonObject* createAddContactJson(const std::string& userName)
        =
            0;

    virtual IActionJsonObject* createAddContactResolutionJson(
        const std::string& userName,
        bool               accepted) = 0;


    virtual IActionJsonObject* createRemoveContactJson(int contactId) = 0;

    virtual IActionJsonObject* createChangeStateJson(USER_STATE state) = 0;

    virtual IActionJsonObject* createUpdateUserJson(const User& user) = 0;
    virtual IActionJsonObject* createRegisterUserJson(const User& user) = 0;

    virtual ~IClientJsonFactory()
    {
    }
};

#endif // ICLIENTJSONFACTORY_HPP

