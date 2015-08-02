#ifndef CLIENTJSONPARSER_HPP
#define CLIENTJSONPARSER_HPP

#include <string>

#include <json/json.h>

#include "IClientJsonParser.hpp"

class ClientJsonParser: public IClientJsonParser
{
public:
    ClientJsonParser();
    ~ClientJsonParser();

    // Implements IJsonParser interface
public:
    bool parseJsonString(const std::string& json);
    Chat_Action_Type getActionType();
    Authentication_Status getAuthenticationStatus();
    UserDetails getUserDetails();
    Contacts getContacts();
private:
    Json::CharReader* p_reader;
    Json::Value m_root;

};

#endif // CLIENTJSONPARSER_HPP
