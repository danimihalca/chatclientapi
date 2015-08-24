#ifndef CLIENTActionFACTORY_HPP
#define CLIENTActionFACTORY_HPP

#include <sstream>

#include "IActionFactory.hpp"

class JsonActionFactory : public IActionFactory
{
public:
//    ClientActionFactory() /*= default*/;

    // Implements IClientActionFactory interface
public:
    IActionObject* createLoginAction(const UserCredentials& userCredentials, USER_STATE state);
    IActionObject* createRequestContactsAction();
    IActionObject* createSendMessageAction(const Message& message);
   IActionObject* createAddContactAction(const std::string& userName);
    IActionObject* createAddContactResolutionAction(
        const std::string& userName,
        bool               accepted);
    IActionObject*createRemoveContactAction(int contactId);
    IActionObject*createChangeStateAction(USER_STATE state);
   IActionObject* createUpdateUserAction(const User& user);
   IActionObject*createRegisterUserAction(const User& user);

};

#endif // CLIENTActionFACTORY_HPP
