#ifndef IWEBSOCKETCLIENTLISTENER_HPP
#define IWEBSOCKETCLIENTLISTENER_HPP

#include <string>

class IWebsocketClientListener
{
public:
    virtual void onMessageReceived(const std::string& message) = 0;
    virtual void onConnected() = 0;
    virtual void onConnectionError() = 0;
    virtual void onDisconnected() = 0;
    virtual ~IWebsocketClientListener()
    {
    }
};


#endif // IWEBSOCKETCLIENTLISTENER_HPP

