#include "JWT.hpp"
#include <jwt-cpp/jwt.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdexcept>

namespace Softadastra
{

    std::string JWT::generate(const std::string &user_id, const std::string &secret_key)
    {
        try
        {
            auto now = std::chrono::system_clock::now();
            auto expiration_time = std::chrono::system_clock::to_time_t(now + std::chrono::hours(1));

            std::string token = jwt::create()
                                    .set_issuer("auth_server")
                                    .set_subject(user_id)
                                    .set_expires_at(std::chrono::system_clock::from_time_t(expiration_time))
                                    .sign(jwt::algorithm::hs256{secret_key});

            return token;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error generating JWT: " << e.what() << std::endl;
            throw;
        }
    }

    bool JWT::verify(const std::string &token, const std::string &secret_key)
    {
        try
        {
            auto decoded_token = jwt::decode(token);

            jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{secret_key})
                .with_issuer("auth_server")
                .verify(decoded_token);

            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error verifying JWT: " << e.what() << std::endl;
            return false;
        }
    }

} // namespace Softadastra
