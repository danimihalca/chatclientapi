#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <memory>

#include "IChatClient.h"

class ChatClientImpl;

class LIBRARY_VISIBILITY ChatClient: public IChatClient
{

public:
    ChatClient();
    void initialize();
    void setNewMessageCallback(newMessageCallback callback);
    void connect(const std::string& uri);
    void startService();
    void sendMessage(const std::string& message);
    void closeConnection();

private:
    std::unique_ptr<ChatClientImpl> p_impl;

};


#endif //CHATCLIENT_H
