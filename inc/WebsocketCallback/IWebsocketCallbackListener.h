#ifndef IWEBSOCKETCALLBACKLISTENER_H
#define IWEBSOCKETCALLBACKLISTENER_H

#include <string>

class IWebsocketCallbackListener
{
public:
    virtual void onMessageReceived(const std::string& message) = 0;
    virtual void onConnected() = 0;
    virtual void onConnectionError() = 0;
    virtual void onDisconnected() = 0;
    virtual ~IWebsocketCallbackListener()
    {
    }
};

#endif // IWEBSOCKETCALLBACKLISTENER_H
