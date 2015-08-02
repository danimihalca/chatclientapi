#include "JsonProtocol/ClientJsonFactory.hpp"

#include <JsonChatProtocol/common_json_protocol.hpp>
#include <Model/User.hpp>

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

