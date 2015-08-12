#ifndef CHATCLIENT_HPP
#define CHATCLIENT_HPP

#include <memory>
#include <list>

#include "ChatClient/IChatClient.hpp"

class IChatClientListener;
class ChatClientImpl;

class LIBRARY_VISIBILITY ChatClient :
    public IChatClient
{

public:
    ChatClient();
    ~ChatClient();

    //Implements IChatClient
public:
    void connect(const std::string& address, uint16_t port);
    void login(const UserCredentials& userCredentials);
    void sendMessage(int receiverId, const std::string& message);
    void requestContacts();
    void disconnect();
    void addContact(const std::string& userName);
    void removeContact(int contactId);

    void addListener(IChatClientListener* listener);
    void removeListener(IChatClientListener* listener);

private:
    //opaque pointer to actual implementation to reduce compile-time dependencies
    std::unique_ptr<ChatClientImpl> p_impl;

};


#endif //CHATCLIENT_HPP
