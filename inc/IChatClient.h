#ifndef ICHATCLIENT_H
#define ICHATCLIENT_H

#include <string>

#include "common.h"

class IChatClient
{
public:
    virtual void initialize() = 0;
    virtual void setNewMessageCallback(newMessageCallback callback) = 0;
    virtual void connect(const std::string& uri = "ws://localhost:9002") = 0;
    virtual void startService() = 0;
    virtual void sendMessage(const std::string& message) = 0;
    virtual void closeConnection() = 0;
    virtual ~IChatClient()
    {
    }
};

#endif //ICHATCLIENT_H

