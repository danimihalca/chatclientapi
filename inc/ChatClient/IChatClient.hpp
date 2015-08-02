#ifndef ICHATCLIENT_HPP
#define ICHATCLIENT_HPP

#include <string>
#include <memory>

#include "chat_client_common.hpp"

class IChatClientListener;

class LIBRARY_VISIBILITY IChatClient
{
public:
    virtual void setServerProperties(const std::string& address,
                                     uint16_t           port) = 0;
    virtual void login(const std::string& user,
                       const std::string& password) = 0;
    virtual void sendMessage(const std::string& message) = 0;
    virtual void disconnect() = 0;
    virtual void addChatClientListener(
        std::shared_ptr<IChatClientListener>& listener) = 0;
    virtual void removeChatClientListener(
        std::shared_ptr<IChatClientListener>& listener) = 0;
    virtual ~IChatClient()
    {
    }
};

#endif //ICHATCLIENT_HPP
