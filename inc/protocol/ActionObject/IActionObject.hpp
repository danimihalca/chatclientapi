#ifndef IACTIONJSONOBJECT_HPP
#define IACTIONJSONOBJECT_HPP

#include <json/json.h>

class IActionObject
{
public:
    virtual std::string toString() = 0;

};

#endif // IACTIONJSONOBJECT_HPP

