#ifndef ACTIONJSONOBJECT_HPP
#define ACTIONJSONOBJECT_HPP

#include "IActionObject.hpp"
#include <memory>

class JsonActionObject: public IActionObject
{
public:
    JsonActionObject(Json::Value root):
        m_root(root)
    {
        if (s_writer.get() == nullptr)
        {
            createWriter();
        }
    }

    std::string toString()
    {
        std::stringstream ss;
        s_writer->write(m_root,&ss);
        return ss.str();
    }

private:
    void createWriter()
    {
        Json::StreamWriterBuilder builder;
        builder["commentStyle"] = "None";
        builder["indentation"] = "";
        builder["enableYAMLCompatibility"] = false;
        builder["dropNullPlaceholders"] = true;

        s_writer.reset(builder.newStreamWriter());
    }

private:
    Json::Value m_root;
    static std::unique_ptr<Json::StreamWriter> s_writer;
};

std::unique_ptr<Json::StreamWriter> JsonActionObject::s_writer;

#endif // ACTIONJSONOBJECT_HPP

