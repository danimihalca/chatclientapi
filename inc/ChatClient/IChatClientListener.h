#ifndef ICHATCLIENTLISTENER_H
#define ICHATCLIENTLISTENER_H

#include <string>

class IChatClientListener
{
public:
    virtual void onMessageReceived(const std::string& message) = 0;
    virtual void onConnected() = 0;
    virtual void onConnectionError() = 0;
    virtual void onDisconnected() = 0;
    virtual ~IChatClientListener()
    {
    }
};

#endif // ICHATCLIENTLISTENER_H

