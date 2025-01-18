#ifndef JWT_HPP
#define JWT_HPP

#include <string>

namespace Softadastra
{
    /**
     * @class JWT
     * @brief A utility class for generating and verifying JWT tokens.
     *
     * This class provides methods for generating and verifying JSON Web Tokens (JWTs).
     * It supports token generation with user identification and expiration,
     * as well as token verification using a secret key.
     */
    class JWT
    {
    public:
        /**
         * @brief Generates a JWT token for the given user ID and secret key.
         *
         * This method creates a JWT token with the following claims:
         * - Issuer set to "auth_server"
         * - Subject set to the provided user ID
         * - Expiration time set to 1 hour from the current time
         *
         * The token is signed using the HMAC-SHA256 algorithm with the provided secret key.
         *
         * @param user_id The ID of the user for whom the token is being generated.
         * @param secret_key The secret key used to sign the JWT token.
         *
         * @return A string representing the generated JWT token.
         *
         * @throw std::exception If an error occurs during token generation (e.g., invalid time or signing).
         */
        static std::string generate(const std::string &user_id, const std::string &secret_key);

        /**
         * @brief Verifies the validity of a given JWT token using the provided secret key.
         *
         * This method decodes the token, checks its signature using the HMAC-SHA256 algorithm
         * with the given secret key, and verifies that the token's issuer matches "auth_server".
         *
         * @param token The JWT token to verify.
         * @param secret_key The secret key used to verify the JWT token's signature.
         *
         * @return true if the token is valid, false if the verification fails (e.g., incorrect signature or expired token).
         *
         * @throw std::exception If an error occurs during token decoding or verification.
         */
        static bool verify(const std::string &token, const std::string &secret_key);
    };
} // namespace Softadastra

#endif // JWT_HPP
