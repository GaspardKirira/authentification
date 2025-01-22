#include "UserValidator.hpp"
#include <algorithm>
#include <cctype>
#include <regex>

bool UserValidator::is_valid_string(const std::string &str)
{
    // Vérifie que la chaîne n'est pas vide
    if (str.empty())
        return false;

    // Vérifie la taille de la chaîne (1 à 100 caractères)
    if (str.length() > 100)
        return false;

    // Vérifie que la chaîne ne commence ni ne termine par un espace
    if (std::isspace(str.front()) || std::isspace(str.back()))
        return false;

    // Vérifie que tous les caractères sont alphabétiques ou des espaces
    return std::all_of(str.begin(), str.end(), [](char ch)
                       { return std::isalpha(ch) || std::isspace(ch); });
}

bool UserValidator::is_valid_email(const std::string &email)
{
    // Regex pour valider une adresse e-mail standard
    const std::regex email_regex(
        R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");

    return std::regex_match(email, email_regex);
}

bool UserValidator::is_valid_city(const std::string &city)
{
    if (city.empty() || city.length() > 100)
        return false;

    // Vérifie que tous les caractères sont alphabétiques ou des espaces
    return std::all_of(city.begin(), city.end(), [](char ch)
                       { return std::isalpha(ch) || std::isspace(ch); });
}

bool UserValidator::is_valid_phone(const std::string &phone)
{
    // Regex pour valider un numéro de téléphone international
    const std::regex phone_regex(R"(^\+?[0-9]{7,15}$)");

    return std::regex_match(phone, phone_regex);
}

bool UserValidator::is_valid_password(const std::string &password)
{
    if (password.length() < 8 || password.length() > 50)
        return false;

    // Vérifie au moins une lettre minuscule, une lettre majuscule, un chiffre et un caractère spécial
    bool has_lower = false, has_upper = false, has_digit = false, has_special = false;
    for (char ch : password)
    {
        if (std::islower(ch))
            has_lower = true;
        else if (std::isupper(ch))
            has_upper = true;
        else if (std::isdigit(ch))
            has_digit = true;
        else if (std::ispunct(ch))
            has_special = true;

        // Si tous les critères sont remplis, on peut arrêter la boucle
        if (has_lower && has_upper && has_digit && has_special)
            return true;
    }

    return false;
}
