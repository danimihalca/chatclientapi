#include "JsonProtocol/JsonFactory.h"

#include "JsonProtocol/common_protocol.h"

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

std::string JsonFactory::createLoginJSON(const std::string& user, const std::string& password)
{
    m_outputStream.str("");
    Json::Value root;
    root[ACTION] = CC_LOGIN_REQUEST;
    Json::Value credentials;
    credentials[USER] = user;
    credentials[PASSWORD] = password;
    root[CREDENTIALS] = credentials;
    p_writer->write(root,&m_outputStream);

    return m_outputStream.str();
}

