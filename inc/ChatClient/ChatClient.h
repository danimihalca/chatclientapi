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
    ChatClient();

    //Implements IChatClient
public:
    void connect(const std::string& address, uint16_t port);
    void sendMessage(const std::string& message);
    void disconnect();
    void addChatClientListener(IChatClientListener* listener);
    void removeChatClientListener(IChatClientListener* listener);

private:
    //opaque pointer to actual implementation to reduce compile-time dependencies
    std::unique_ptr<ChatClientImpl> p_impl;

};


#endif //CHATCLIENT_H
