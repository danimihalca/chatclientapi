#ifndef IJSONPARSER
#define IJSONPARSER

#include <string>

#include <common_protocol.h>

class IJsonParser
{
public:
    virtual ~IJsonParser()
    {
    }

    virtual void parseJsonString(const std::string& json) = 0 ;
    virtual ChatServer_Action_Type getActionType() = 0;
    virtual Authentification_Status getAuthentificationStatus() = 0;
};

#endif // IJSONPARSER

