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

std::string ClientJsonFactory::createLoginJsonString(
    const UserCredentials& userCredentials)
{
    m_outputStream.str("");
    Json::Value root;
    root[REQUEST_ACTION] = REQUEST_LOGIN;
    Json::Value content;
    Json::Value userCredentialsJson;
    userCredentialsJson[USERNAME] = userCredentials.getUserName();
    userCredentialsJson[PASSWORD] = userCredentials.getPassword();
    content[USER_CREDENTIALS] = userCredentialsJson;

    root[CONTENT] = content;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}


std::string ClientJsonFactory::createRequestContactsJsonString()
{
    m_outputStream.str("");
    Json::Value root;
    root[REQUEST_ACTION] = REQUEST_GET_CONTACTS;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}

std::string ClientJsonFactory::createSendMessageJsonString(
    const Message& message)
{
    m_outputStream.str("");
    Json::Value root;
    root[REQUEST_ACTION] = REQUEST_SEND_MESSAGE;
    Json::Value content;

    Json::Value messageJson;

    messageJson[RECEIVER_ID] = message.getReceiverId();
    messageJson[MESSAGE_TEXT] = message.getMessageText();

    root[CONTENT] = content;
    content[MESSAGE] = messageJson;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}
