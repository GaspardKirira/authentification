#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <string>

namespace Softadastra
{

    class Profile
    {
    public:
        explicit Profile(const std::string &username, const std::string &lastname, const std::string &city);

        Profile(const Profile &rhs);
        Profile &operator=(const Profile &rhs);
        Profile(Profile &&rhs);
        Profile &operator=(Profile &&rhs);
        virtual ~Profile() = default;

    protected:
        int m_id{};
        std::string m_username{};
        std::string m_lastname{};
        std::string m_city{};
    };

} // namespace Softadastra

#endif // PROFILE_HPP