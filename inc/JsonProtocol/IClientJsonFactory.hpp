#ifndef ICLIENTJSONFACTORY_HPP
#define ICLIENTJSONFACTORY_HPP

#include <string>

class UserCredentials;
class User;

class IClientJsonFactory
{
public:
    virtual std::string createLoginJsonString(
        const UserCredentials& userCredentials) = 0;
    virtual std::string createGetContactsRequestJsonString(const User& user) =
        0;


    virtual ~IClientJsonFactory()
    {
    }
};

#endif // ICLIENTJSONFACTORY_HPP

