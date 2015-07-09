#ifndef CHATCLIENTIMPL_H
#define CHATCLIENTIMPL_H

#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

using asioClient = websocketpp::client<websocketpp::config::asio_client>;

class ChatClientImpl
{

public:
    void initialize();
    void connect(const std::string& uri);
    void startService();
    void sendMessage(const std::string& message);
    void closeConnection();
    ~ChatClientImpl();

private:
    void setHandlers();
    void setLogAccessChannels();
    void onOpen(websocketpp::connection_hdl);
    void onFail(websocketpp::connection_hdl);
    void onClose(websocketpp::connection_hdl);
    void onMessage(websocketpp::connection_hdl, asioClient::message_ptr msg);
private:
    asioClient m_client;
    websocketpp::connection_hdl m_connectionHandle;
    bool m_connected = false;
    std::shared_ptr<websocketpp::lib::thread> p_serviceThread;
};


#endif //CHATCLIENTIMPL_H
