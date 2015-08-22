#ifndef CLIENTJSONFACTORY_HPP
#define CLIENTJSONFACTORY_HPP

#include <json/json.h>
#include <sstream>

#include "IClientJsonFactory.hpp"

class ClientJsonFactory : public IClientJsonFactory
{
public:
    ClientJsonFactory() = default;

    // Implements IClientJsonFactory interface
public:
    IActionJsonObject* createLoginJson(const UserCredentials& userCredentials, USER_STATE state);
    IActionJsonObject* createRequestContactsJson();
    IActionJsonObject* createSendMessageJson(const Message& message);
   IActionJsonObject* createAddContactJson(const std::string& userName);
    IActionJsonObject* createAddContactResolutionJson(
        const std::string& userName,
        bool               accepted);
    IActionJsonObject*createRemoveContactJson(int contactId);
    IActionJsonObject*createChangeStateJson(USER_STATE state);
   IActionJsonObject* createUpdateUserJson(const User& user);
   IActionJsonObject*createRegisterUserJson(const User& user);

};

#endif // CLIENTJSONFACTORY_HPP
