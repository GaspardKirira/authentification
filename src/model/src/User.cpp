#include "User.hpp"

namespace Softadastra
{
    User::User(const std::string &username, const std::string &lastname, const std::string &city, const std::string &email, const std::string &phone, const std::string &password)
        : Profile(username, lastname, city), Contact(email, phone), m_password_hash(password) {}

    User::User(const User &user)
        : Profile(user.m_username, user.m_lastname, user.m_city), Contact(user.m_email, user.m_phone), m_password_hash(user.m_password_hash) {}

    User &User::operator=(const User &user)
    {
        if (this != &user)
        {
            Profile::operator=(user);
            Contact::operator=(user);
            m_password_hash = user.m_password_hash;
        }
        return *this;
    }

    User::User(User &&user) noexcept
        : Profile(std::move(user.m_username), std::move(user.m_lastname), std::move(user.m_city)),
          Contact(std::move(user.m_email), std::move(user.m_phone)),
          m_password_hash(std::move(user.m_password_hash)) {}

    User &User::operator=(User &&user) noexcept
    {
        if (this != &user)
        {
            Profile::operator=(std::move(user));
            Contact::operator=(std::move(user));
            m_password_hash = std::move(user.m_password_hash);
        }
        return *this;
    }
}
