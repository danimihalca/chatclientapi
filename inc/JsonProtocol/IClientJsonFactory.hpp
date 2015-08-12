#ifndef ICLIENTJSONFACTORY_HPP
#define ICLIENTJSONFACTORY_HPP

#include <string>

class UserCredentials;
class User;

class Message;

class IClientJsonFactory
{
public:
    virtual std::string createLoginJsonString(
        const UserCredentials& userCredentials) = 0;
    virtual std::string createRequestContactsJsonString() = 0;
    virtual std::string createSendMessageJsonString(const Message& message) =
        0;

    virtual std::string createAddContactJsonString(const std::string& userName)
        =
            0;

    virtual std::string createAddContactResolutionJsonString(
        const std::string& userName,
        bool               accepted) = 0;


    virtual std::string createRemoveContactJsonString(int contactId) = 0;

    virtual ~IClientJsonFactory()
    {
    }
};

#endif // ICLIENTJSONFACTORY_HPP

