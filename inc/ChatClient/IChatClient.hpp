#ifndef ICHATCLIENT_HPP
#define ICHATCLIENT_HPP

#include <string>
#include <memory>

#include "chat_client_common.hpp"

class IChatClientListener;

class UserCredentials;


class LIBRARY_VISIBILITY IChatClient
{
public:
    virtual void connect(const std::string& address, uint16_t port) = 0;
    virtual void login(const UserCredentials& userCredentials) = 0;
    virtual void sendMessage(int receiverId, const std::string& message) = 0;
    virtual void requestContacts() = 0;
    virtual void disconnect() = 0;
    virtual void addContact(const std::string& userName) = 0;
    virtual void removeContact(int contactId) = 0;
    virtual void addListener(IChatClientListener* listener) = 0;
    virtual void removeListener(IChatClientListener* listener) = 0;

    virtual ~IChatClient()
    {
    }
};

#endif //ICHATCLIENT_HPP
