#include "Profile.hpp"

namespace Softadastra
{
    Profile::Profile(const std::string &username, const std::string &lastname, const std::string &city)
        : m_username(username), m_lastname(lastname), m_city(city) {}

    Profile::Profile(const Profile &rhs)
        : m_username(rhs.m_username), m_lastname(rhs.m_lastname), m_city(rhs.m_city) {}

    Profile &Profile::operator=(const Profile &rhs)
    {
        if (this != &rhs)
        {
            m_username = rhs.m_username;
            m_lastname = rhs.m_lastname;
            m_city = rhs.m_city;
        }
        return *this;
    }

    Profile::Profile(Profile &&rhs)
        : m_username(std::move(rhs.m_username)), m_lastname(std::move(rhs.m_lastname)), m_city(std::move(rhs.m_city)) {}

    Profile &Profile::operator=(Profile &&rhs)
    {
        if (this != &rhs)
        {
            m_username = std::move(rhs.m_username);
            m_lastname = std::move(rhs.m_lastname);
            m_city = std::move(std::move(rhs.m_city));
        }
        return *this;
    }

} // namespace Softadastra
