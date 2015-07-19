#ifndef ICHATCLIENT_H
#define ICHATCLIENT_H

#include <string>

#include "chat_client_common.h"

class IChatClientListener;

class LIBRARY_VISIBILITY IChatClient
{
public:
    virtual void connect(const std::string& address, uint16_t port) = 0;
    virtual void sendMessage(const std::string& message) = 0;
    virtual void disconnect() = 0;
    virtual void addChatClientListener(IChatClientListener* listener) = 0;
    virtual void removeChatClientListener(IChatClientListener* listener) = 0;
    virtual ~IChatClient()
    {
    }
};

#endif //ICHATCLIENT_H
