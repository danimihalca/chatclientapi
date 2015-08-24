#ifndef CONTACTSTATECHANGEDJSON_HPP
#define CONTACTSTATECHANGEDJSON_HPP

#include "Model/User.hpp"

class ContactStateChangedNotification
{

public:
    ContactStateChangedNotification() = default;

    int getContactId() const
    {
        return m_contactId;
    }
    void setContactId(int contactId)
    {
        m_contactId = contactId;
    }

    USER_STATE getContactState() const
    {
        return m_state;
    }
    void setContactState(const USER_STATE& state)
    {
        m_state = state;
    }

private:
    int m_contactId;
    USER_STATE m_state;
};

#endif // CONTACTSTATECHANGEDJSON_HPP









