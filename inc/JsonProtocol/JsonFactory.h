#ifndef JSONFACTORY_H
#define JSONFACTORY_H

#include <json/json.h>
#include <sstream>

class JsonFactory
{
public:
    JsonFactory();
    ~JsonFactory();

    std::string createLoginJSON(const std::string& user, const std::string& password);

private:
    Json::StreamWriter* p_writer;
    std::stringstream m_outputStream;
};

#endif // JSONFACTORY_H

