#ifndef IWEBSOCKETCLIENT_H
#define IWEBSOCKETCLIENT_H

#include <string>

#include "chat_client_common.h"

class IWebsocketClientListener;

class IWebsocketClient
{
public:
    virtual void initialize() = 0;
    virtual void connect(const std::string& address, uint16_t port) = 0;
    virtual void startService() = 0;
    virtual void sendMessage(const std::string& message) = 0;
    virtual void closeConnection() = 0;

    virtual void addWebsocketClientListener(IWebsocketClientListener* listener) = 0;
    virtual void removeWebsocketClientListener(IWebsocketClientListener* listener) = 0;

    virtual ~IWebsocketClient()
    {
    }
};


#endif // IWEBSOCKETCLIENT_H

