#ifndef IWEBSOCKETCLIENTLISTENER_H
#define IWEBSOCKETCLIENTLISTENER_H

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


#endif // IWEBSOCKETCLIENTLISTENER_H

