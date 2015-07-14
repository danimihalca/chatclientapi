#ifndef CHATCLIENTIMPL_H
#define CHATCLIENTIMPL_H

#include <string>
#include <libwebsockets.h>
#include <memory>
#include "common.h"
#include <thread>
static const int MAX_ECHO_PAYLOAD = 1400;

class ChatClientImpl;

struct per_session_data__echo
{
    unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + MAX_ECHO_PAYLOAD + LWS_SEND_BUFFER_POST_PADDING];
    unsigned int len;
    unsigned int index;
    ChatClientImpl* client;
};

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
    int callback(struct libwebsocket_context *context,
                  struct libwebsocket *wsi,
                  enum libwebsocket_callback_reasons reason, void *user,
                  void *in, size_t len);

private:
    void run();
private:
    newMessageCallback m_newMessageCB;
    struct libwebsocket_context *m_context;
    std::unique_ptr<struct libwebsocket_protocols> m_protocols;
    struct libwebsocket *wsi;
    struct per_session_data__echo data;
    std::thread m_thread;
    bool m_running;
public:
    bool m_connected = false;
};


int callback2(struct libwebsocket_context *context,
              struct libwebsocket *wsi,
              enum libwebsocket_callback_reasons reason, void *user,
              void *in, size_t len);

#endif //CHATCLIENTIMPL_H
