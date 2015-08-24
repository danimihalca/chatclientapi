#ifndef ICLIENTActionFACTORY_HPP
#define ICLIENTActionFACTORY_HPP

#include <string>

#include <Model/User.hpp>

class IActionObject;
class Message;

class IActionFactory
{
public:
    virtual IActionObject* createLoginAction(
        const UserCredentials& userCredentials,  USER_STATE state) = 0;
    virtual IActionObject* createRequestContactsAction() = 0;
    virtual IActionObject* createSendMessageAction(const Message& message) =
        0;

    virtual IActionObject* createAddContactAction(const std::string& userName)
        =
            0;

    virtual IActionObject* createAddContactResolutionAction(
        const std::string& userName,
        bool               accepted) = 0;


    virtual IActionObject* createRemoveContactAction(int contactId) = 0;

    virtual IActionObject* createChangeStateAction(USER_STATE state) = 0;

    virtual IActionObject* createUpdateUserAction(const User& user) = 0;
    virtual IActionObject* createRegisterUserAction(const User& user) = 0;

    virtual ~IActionFactory()
    {
    }
};

#endif // ICLIENTActionFACTORY_HPP

