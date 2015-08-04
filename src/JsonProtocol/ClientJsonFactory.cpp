#include "JsonProtocol/ClientJsonFactory.hpp"

#include <JsonChatProtocol/common_json_protocol.hpp>
#include <Model/User.hpp>
#include <Model/Message.hpp>

ClientJsonFactory::ClientJsonFactory()
{
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    builder["enableYAMLCompatibility"] = false;
    builder["dropNullPlaceholders"] = true;

    p_writer = builder.newStreamWriter();
}

ClientJsonFactory::~ClientJsonFactory()
{
    delete p_writer;
}

std::string ClientJsonFactory::createLoginJsonString(const UserCredentials& userCredentials)
{
    m_outputStream.str("");
    Json::Value root;
    root[ACTION] = LOGIN_REQUEST;
    Json::Value userCredentialsJson;
    userCredentialsJson[USER_USERNAME] = userCredentials.getUserName();
    userCredentialsJson[USER_PASSWORD] = userCredentials.getPassword();
    root[USER_CREDENTIALS] = userCredentialsJson;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}


std::string ClientJsonFactory::createGetContactsRequestJsonString(const User& user)
{
    m_outputStream.str("");
    Json::Value root;
    root[ACTION] = GET_CONTACTS_REQUEST;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}

std::string ClientJsonFactory::createSendMessageJsonString(const Message& message)
{
    m_outputStream.str("");
    Json::Value root;
    root[ACTION] = SEND_MESSAGE;

    Json::Value messageJson;

    messageJson[MESSAGE_RECEIVER_ID] = message.getReceiverId();
    messageJson[MESSAGE_TEXT] = message.getMessageText();

    root[MESSAGE] = messageJson;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}
