#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include "Contact.hpp"
#include "Profile.hpp"

namespace Softadastra
{
    class User : public Profile, public Contact
    {
    public:
        explicit User(const std::string &username, const std::string &lastname, const std::string &city, const std::string &email, const std::string &phone, const std::string &password);
        User(const User &user);
        User &operator=(const User &user);
        User(User &&user) noexcept;
        User &operator=(User &&user) noexcept;
        ~User() = default;
        void display() const
        {
            std::cout << "Username: " << Profile::m_username << "\n"
                      << "Lastname: " << Profile::m_lastname << "\n"
                      << "City: " << Profile::m_city << "\n"
                      << "Email: " << m_email << "\n"
                      << "Phone: " << m_phone << "\n"
                      << "Password: " << m_password_hash << std::endl;
        }

    private:
        int m_id{};
        std::string m_password_hash{};
        std::string m_create_at{};
        std::string m_updated_at{};
    };
} // namespace Softadastra

#endif // USER_HPP
