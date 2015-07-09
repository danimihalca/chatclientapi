#include "ChatClientImpl.h"


void ChatClientImpl::initialize()
{
    m_client.init_asio();
    setHandlers();
    setLogAccessChannels();
}

void ChatClientImpl::connect(const std::string& uri)
{
    websocketpp::lib::error_code errorCode;
    asioClient::connection_ptr connection = m_client.get_connection(uri, errorCode);
    if (errorCode)
    {
        m_client.get_alog().write(websocketpp::log::alevel::app, "Get Connection Error: " + errorCode.message());
        return;
    }

    m_client.connect(connection);
    m_connectionHandle = connection->get_handle();
//    m_client.get_alog().write(websocketpp::log::alevel::app, "Connecting...");
//    m_serviceThread = websocketpp::lib::thread(&asioClient::run, &m_client);


}

void ChatClientImpl::startService()
{
    m_client.run();
}



void ChatClientImpl::sendMessage(const std::string& message)
{
    while (!m_connected)
    {
        sleep(1);
    }
//    m_client.get_alog().write(websocketpp::log::alevel::app, "Sending Message: \"" + message +'\"');
    websocketpp::lib::error_code errorCode;

    m_client.send(m_connectionHandle, message, websocketpp::frame::opcode::text, errorCode);
    if (errorCode)
    {
        m_client.get_alog().write(websocketpp::log::alevel::app, "Sending Message Error: " + errorCode.message());
    }
}

void ChatClientImpl::closeConnection()
{
    if (!m_client.stopped())
    {
        m_client.stop();
    }
//    if (m_serviceThread.joinable())
//    {
//        m_serviceThread.join();
//    }
//    m_serviceThread.join();
}

//ChatClientImpl::~ChatClientImpl()
//{
//    if (!m_client.stopped())
//    {
//        closeConnection();
//    }
//}

void ChatClientImpl::setHandlers()
{
    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;
    using websocketpp::lib::bind;

    m_client.set_open_handler(bind(&ChatClientImpl::onOpen, this, _1));
    m_client.set_close_handler(bind(&ChatClientImpl::onClose, this, _1));
    m_client.set_fail_handler(bind(&ChatClientImpl::onFail, this, _1));
    m_client.set_message_handler(bind(&ChatClientImpl::onMessage, this, _1, _2));
}

void ChatClientImpl::setLogAccessChannels()
{
    m_client.clear_access_channels(websocketpp::log::alevel::all);
//    m_client.set_access_channels(websocketpp::log::alevel::connect);
//    m_client.set_access_channels(websocketpp::log::alevel::disconnect);
//    m_client.set_access_channels(websocketpp::log::alevel::app);
}

void ChatClientImpl::onOpen(websocketpp::connection_hdl)
{
    m_client.get_alog().write(websocketpp::log::alevel::app, "Connection opened!");
    m_connected = true;
}

void ChatClientImpl::onFail(websocketpp::connection_hdl)
{
    m_client.get_alog().write(websocketpp::log::alevel::app, "Connection failed!");
}

void ChatClientImpl::onClose(websocketpp::connection_hdl)
{
    m_client.get_alog().write(websocketpp::log::alevel::app, "Connection closed!");
    m_connected = false;
}

void ChatClientImpl::onMessage(websocketpp::connection_hdl,asioClient::message_ptr msg)
{
    m_client.get_alog().write(websocketpp::log::alevel::app, "Message received!!");
    m_client.get_alog().write(websocketpp::log::alevel::app, "Message:" + msg->get_payload());
    std::cout<<"MESSAGE: "<<msg->get_payload()<<std::endl;
}
