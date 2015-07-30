#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>

#include <json/json.h>

#include "IJsonParser.h"

class JsonParser: public IJsonParser
{
public:
    JsonParser();
    ~JsonParser();

    //Implements IJsonParser
public:
    void parseJsonString(const std::string& json);
    ChatServer_Action_Type getActionType();
    Authentification_Status getAuthentificationStatus();

private:
    Json::CharReader* p_reader;
    Json::Value m_root;

};

#endif // JSONPARSER_H

