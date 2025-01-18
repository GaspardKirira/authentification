#include "Contact.hpp"

namespace Softadastra
{
    Contact::Contact(const std::string &email, const std::string &phone)
        : m_email(email), m_phone(phone) {}

    Contact::Contact(const Contact &rhs)
        : m_email(rhs.m_email), m_phone(rhs.m_phone) {}

    Contact &Contact::operator=(const Contact &rhs)
    {
        if (this != &rhs)
        {
            m_email = rhs.m_email;
            m_phone = rhs.m_phone;
        }
        return *this;
    }

    Contact::Contact(Contact &&rhs) noexcept
        : m_email(std::move(rhs.m_email)), m_phone(std::move(rhs.m_phone)) {}

    Contact &Contact::operator=(Contact &&rhs) noexcept
    {
        if (this != &rhs)
        {
            m_email = std::move(rhs.m_email);
            m_phone = std::move(rhs.m_phone);
        }
        return *this;
    }

    Contact::~Contact() = default;
}
