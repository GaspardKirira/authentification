#ifndef USER_HPP
#define USER_HPP

#include <string>

class User
{
public:
    explicit User(const std::string &username, const std::string &lastname, const std::string &city, const std::string &phone, const std::string &password)
        : m_username(username), m_lastname(lastname), m_city(city), m_phone(phone), m_password_hash(password) {}

    explicit User(const User &user)
        : m_username(user.m_username), m_lastname(user.m_lastname), m_city(user.m_city), m_phone(user.m_phone) {}

    User &operator=(const User &user)
    {
        if (this != &user)
        {
            m_username = user.m_username;
            m_lastname = user.m_lastname;
            m_city = user.m_city;
            m_phone = user.m_phone;
        }
        return *this;
    }

    User(User &&user)
        : m_username(std::move(user.m_username)), m_lastname(std::move(user.m_lastname)), m_city(std::move(user.m_city)), m_phone(std::move(user.m_phone)) {}

    User &operator=(User &&user)
    {
        if (this != &user)
        {
            m_username = std::move(user.m_username);
            m_lastname = std::move(user.m_lastname);
            m_city = std::move(user.m_city);
            m_phone = std::move(user.m_phone);
        }
        return *this;
    }

    ~User() = default;

private:
    int m_id{};
    std::string m_username{};
    std::string m_lastname{};
    std::string m_city{};
    std::string m_phone{};
    std::string m_password_hash{};
    std::string m_create_at{};
    std::string m_updated_at{};
};

#endif // USER_HPP