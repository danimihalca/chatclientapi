#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <memory>
#include <list>

#include "ChatClient/IChatClient.h"

class IChatClientListener;
class ChatClientImpl;

class LIBRARY_VISIBILITY ChatClient :
    public IChatClient
{

public:
    ChatClient(const std::string& address, uint16_t port);
    ~ChatClient();

    //Implements IChatClient
public:
    void login(const std::string& user, const std::string& password);
    void sendMessage(const std::string& message);
    void disconnect();
    void addChatClientListener(std::shared_ptr<IChatClientListener>& listener);
    void removeChatClientListener(std::shared_ptr<IChatClientListener>& listener);

private:
    //opaque pointer to actual implementation to reduce compile-time dependencies
    std::unique_ptr<ChatClientImpl> p_impl;

};


#endif //CHATCLIENT_H
