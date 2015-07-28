#ifndef CHATCLIENTIMPL_H
#define CHATCLIENTIMPL_H

#include <memory>
#include <list>

#include "ChatClient/IChatClient.h"
#include "WebsocketClient/IWebsocketClientListener.h"

class IWebsocketClient;

class IChatClientListener;

class ChatClientImpl :
    public IChatClient,
    public IWebsocketClientListener
{

public:
    ChatClientImpl();
    ~ChatClientImpl();

    //Implements IChatClient
public:
    void connect(const std::string& address, uint16_t port);
    void sendMessage(const std::string& message);
    void disconnect();
    void addChatClientListener(std::shared_ptr<IChatClientListener>& listener);
    void removeChatClientListener(std::shared_ptr<IChatClientListener>& listener);

    //Implements IWebsocketClientListener
public:
    void onMessageReceived(const std::string& message);
    void onConnected();
    void onDisconnected();
    void onConnectionError();

private:
    std::unique_ptr<IWebsocketClient> p_websocketClient;
    std::list<std::shared_ptr<IChatClientListener> > m_clientListeners;

};


#endif //CHATCLIENTIMPL_H
