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
    void setServerProperties(const std::string& address, uint16_t port);
    void login(const std::string& user, const std::string& password);
    void sendMessage(int receiverId, const std::string& message);
    void getContacts();
    void disconnect();
    void addListener(std::shared_ptr<IChatClientListener>& listener);
    void removeListener(std::shared_ptr<IChatClientListener>& listener);

private:
    //opaque pointer to actual implementation to reduce compile-time dependencies
    std::unique_ptr<ChatClientImpl> p_impl;

};


#endif //CHATCLIENT_HPP
