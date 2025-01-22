#ifndef USERVALIDATOR_HPP
#define USERVALIDATOR_HPP

#include <string>

class UserValidator
{
public:
    static bool is_valid_string(const std::string &username);
    static bool is_valid_email(const std::string &email);
    static bool is_valid_city(const std::string &city);
    static bool is_valid_phone(const std::string &phone);
    static bool is_valid_password(const std::string &password);
};

#endif // USERVALIDATOR_HPP