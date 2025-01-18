#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <string>

namespace Softadastra
{
    class Contact
    {
    public:
        explicit Contact(const std::string &email, const std::string &phone);
        Contact(const Contact &rhs);
        Contact &operator=(const Contact &rhs);
        Contact(Contact &&rhs) noexcept;
        Contact &operator=(Contact &&rhs) noexcept;
        virtual ~Contact();

    protected:
        std::string m_email{};
        std::string m_phone{};
    };
} // namespace Softadastra

#endif // CONTACT_HPP