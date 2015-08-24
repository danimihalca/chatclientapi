#ifndef REMOVEDBYCONTACTCONTACTJSON_HPP
#define REMOVEDBYCONTACTCONTACTJSON_HPP

#include <string>

class RemovedByContactNotification
{
public:
        RemovedByContactNotification() = default;
        inline RemovedByContactNotification(int contactId) :
                m_contactId(contactId)
        {
        }

        const int getContactId() const
        {
                return m_contactId;
        }

private:
        int m_contactId;
};

#endif //REMOVEDBYCONTACTCONTACTJSON_HPP
