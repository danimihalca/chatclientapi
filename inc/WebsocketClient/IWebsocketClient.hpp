#ifndef IWEBSOCKETCLIENT_HPP
#define IWEBSOCKETCLIENT_HPP

#include <string>

#include "chat_client_common.hpp"

class IWebsocketClientListener;

class IWebsocketClient
{
public:
    virtual void initialize() = 0;
    virtual void setServer(const std::string& address, uint16_t port) = 0;
    virtual void connect() = 0;
    virtual void startService() = 0;
    virtual void sendText(const std::string& text) = 0;
    virtual void closeConnection() = 0;

    virtual void addListener(IWebsocketClientListener* listener) = 0;
    virtual void removeListener(IWebsocketClientListener* listener) = 0;

    virtual ~IWebsocketClient()
    {
    }
};


#endif // IWEBSOCKETCLIENT_HPP

