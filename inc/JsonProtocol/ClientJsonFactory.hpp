#ifndef CLIENTJSONFACTORY_HPP
#define CLIENTJSONFACTORY_HPP

#include <json/json.h>
#include <sstream>

#include "IClientJsonFactory.hpp"

class ClientJsonFactory: public IClientJsonFactory
{
public:
    ClientJsonFactory();
    ~ClientJsonFactory();

    std::string createLoginJsonString(const UserCredentials& userCredentials);

private:
    Json::StreamWriter* p_writer;
    std::stringstream m_outputStream;
};

#endif // CLIENTJSONFACTORY_HPP

