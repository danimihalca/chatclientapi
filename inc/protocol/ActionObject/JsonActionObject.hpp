#ifndef ACTIONJSONOBJECT_HPP
#define ACTIONJSONOBJECT_HPP

#include "IActionObject.hpp"
#include <memory>

#include <json/json.h>

class JsonActionObject: public IActionObject
{
public:
    JsonActionObject(Json::Value root):
        m_root(root)
    {

    }

    std::string toString()
    {
        std::stringstream ss;
        getWriterInstance()->write(m_root,&ss);
        return ss.str();
    }

protected:
    static Json::StreamWriter* getWriterInstance()
    {
        if (s_writer.get() == nullptr)
        {
            Json::StreamWriterBuilder builder;
            builder["commentStyle"] = "None";
            builder["indentation"] = "";
            builder["enableYAMLCompatibility"] = false;
            builder["dropNullPlaceholders"] = true;

            s_writer.reset(builder.newStreamWriter());
        }
        return s_writer.get();
    }

private:
    Json::Value m_root;
    static std::unique_ptr<Json::StreamWriter> s_writer;
};

std::unique_ptr<Json::StreamWriter> JsonActionObject::s_writer;

#endif // ACTIONJSONOBJECT_HPP

