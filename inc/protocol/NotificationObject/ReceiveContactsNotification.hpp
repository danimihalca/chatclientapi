#ifndef RECEIVECONTACTSJSON_HPP
#define RECEIVECONTACTSJSON_HPP

#include <vector>

#include <Model/User.hpp>

class ReceiveContactsNotification
{
public:
    ReceiveContactsNotification() = default;

    const std::vector<Contact>& getContacts() const
    {
        return m_contacts;
    }
    void setContacts(const std::vector<Contact>& contacts)
    {
        m_contacts = contacts;
    }

private:
    std::vector<Contact> m_contacts;

};

#endif // RECEIVECONTACTSJSON_HPP





