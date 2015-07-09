#include "ChatClientImpl.h"

#ifdef ANDROID
#include "android/log.h"
#endif

void ChatClientImpl::initialize()
{
    m_client.init_asio();
    setHandlers();
    setLogAccessChannels();
    m_client.start_perpetual();
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
    m_client.get_alog().write(websocketpp::log::alevel::app, "Connecting...");


}

void ChatClientImpl::startService()
{
    p_serviceThread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&asioClient::run, &m_client);
}



void ChatClientImpl::sendMessage(const std::string& message)
{
    while (!m_connected)
    {
        sleep(1);
    }
    m_client.get_alog().write(websocketpp::log::alevel::app, "Sending Message: \"" + message +'\"');
    websocketpp::lib::error_code errorCode;

    m_client.send(m_connectionHandle, message, websocketpp::frame::opcode::text, errorCode);
    if (errorCode)
    {
        m_client.get_alog().write(websocketpp::log::alevel::app, "Sending Message Error: " + errorCode.message());
    }
}

void ChatClientImpl::closeConnection()
{
    m_connected = false;
    websocketpp::lib::error_code errorCode;
    m_client.close(m_connectionHandle,websocketpp::close::status::normal,"disconnected");
}

ChatClientImpl::~ChatClientImpl()
{
    m_client.stop_perpetual();
    if (m_connected)
    {
        closeConnection();
    }
    if (p_serviceThread->joinable())
    {
        p_serviceThread->join();
    }
}

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
    m_client.set_access_channels(websocketpp::log::alevel::connect);
    m_client.set_access_channels(websocketpp::log::alevel::disconnect);
    m_client.set_access_channels(websocketpp::log::alevel::app);
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
