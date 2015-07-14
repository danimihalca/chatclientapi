#ifndef CHATCLIENTIMPL_H
#define CHATCLIENTIMPL_H

#include <string>
#include <libwebsockets.h>

#include "common.h"

class ChatClientImpl
{

public:
    void initialize();
    void setNewMessageCallback(newMessageCallback callback);
    void connect(const std::string& uri);
    void startService();
    void sendMessage(const std::string& message);
    void closeConnection();
    ~ChatClientImpl();

private:
    bool m_connected = false;
    newMessageCallback m_newMessageCB;
};


#endif //CHATCLIENTIMPL_H
