#ifndef IWEBSOCKETCLIENT_HPP
#define IWEBSOCKETCLIENT_HPP

#include <string>

#include "chat_client_common.hpp"

class IWebsocketClientListener;

class IWebsocketClient
{
public:
    virtual void setServerProperties(const std::string& address,
                                     uint16_t           port) = 0;
    virtual void initialize() = 0;
    virtual bool connect() = 0;
    virtual void startService() = 0;
    virtual void sendMessage(const std::string& message) = 0;
    virtual void closeConnection() = 0;

    virtual void addWebsocketClientListener(IWebsocketClientListener* listener)
        = 0;
    virtual void removeWebsocketClientListener(
        IWebsocketClientListener* listener) = 0;

    virtual ~IWebsocketClient()
    {
    }
};


#endif // IWEBSOCKETCLIENT_HPP

