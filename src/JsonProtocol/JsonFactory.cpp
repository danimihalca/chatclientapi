#include "JsonProtocol/JsonFactory.h"

#include <JsonChatProtocol/common_json_protocol.h>

JsonFactory::JsonFactory()
{
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    builder["enableYAMLCompatibility"] = false;
    builder["dropNullPlaceholders"] = true;

    p_writer = builder.newStreamWriter();
}

JsonFactory::~JsonFactory()
{
    delete p_writer;
}

std::string JsonFactory::createLoginJSON(const std::string& username, const std::string& password)
{
    m_outputStream.str("");
    Json::Value root;
    root[ACTION] = CC_LOGIN_REQUEST;
    Json::Value user;
    user[USERNAME] = username;
    user[PASSWORD] = password;
    root[USER] = user;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}

