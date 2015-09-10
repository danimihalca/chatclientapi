#ifndef ADDINGBYCONTACTJSON_HPP
#define ADDINGBYCONTACTJSON_HPP

#include <string>

class AddRequestNotification
{
public:
    AddRequestNotification() = default;
    inline AddRequestNotification(const std::string& userName) :
        m_userName(userName)
    {
    }

    const std::string& getUserName() const
    {
        return m_userName;
    }

private:
    std::string m_userName;
};

#endif //ADDINGBYCONTACTJSON_HPP
