#ifndef IWEBSOCKETCALLBACKLISTENER_HPP
#define IWEBSOCKETCALLBACKLISTENER_HPP

#include <string>

class IWebsocketCallbackListener
{
public:
    virtual void onTextReceived(const std::string& message) = 0;
    virtual void onConnected() = 0;
    virtual void onConnectionError() = 0;
    virtual void onDisconnected() = 0;
    virtual ~IWebsocketCallbackListener()
    {
    }
};

#endif // IWEBSOCKETCALLBACKLISTENER_HPP
