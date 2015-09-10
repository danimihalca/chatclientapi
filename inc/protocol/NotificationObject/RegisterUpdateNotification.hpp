#ifndef REGISTERUPDATEUSERRESPONSEJSON_HPP
#define REGISTERUPDATEUSERRESPONSEJSON_HPP

#include <JsonChatProtocol/common_json_protocol.hpp>

class RegisterUpdateNotification
{
public:
    RegisterUpdateNotification() = default;

    RegisterUpdateNotification(REGISTER_UPDATE_USER_STATUS status) :
        m_status(status)
    {
    }


    inline REGISTER_UPDATE_USER_STATUS getStatus() const
    {
        return m_status;
    }

    inline void setStatus(REGISTER_UPDATE_USER_STATUS status)
    {
        m_status = status;
    }

private:
    REGISTER_UPDATE_USER_STATUS m_status;

};


#endif // REGISTERUPDATEUSERRESPONSEJSON_HPP





