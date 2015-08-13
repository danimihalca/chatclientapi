#ifndef CLIENTJSONFACTORY_HPP
#define CLIENTJSONFACTORY_HPP

#include <json/json.h>
#include <sstream>

#include "IClientJsonFactory.hpp"

class ClientJsonFactory : public IClientJsonFactory
{
public:
    ClientJsonFactory();
    ~ClientJsonFactory();

    // Implements IClientJsonFactory interface
public:
    std::string createLoginJsonString(const UserCredentials& userCredentials, USER_STATE state);
    std::string createRequestContactsJsonString();
    std::string createSendMessageJsonString(const Message& message);
    std::string createAddContactJsonString(const std::string& userName);
    std::string createAddContactResolutionJsonString(
        const std::string& userName,
        bool               accepted);
    std::string createRemoveContactJsonString(int contactId);
    std::string createChangeStateJsonString(USER_STATE state);
    std::string createUpdateUserJsonString(const User& user);
    std::string createRegisterUserJsonString(const User& user);

private:
    Json::StreamWriter* p_writer;
    std::stringstream m_outputStream;

};

#endif // CLIENTJSONFACTORY_HPP
