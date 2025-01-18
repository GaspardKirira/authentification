#ifndef SOFTADASTRA_AUTH_MIDDLEWARE_HPP
#define SOFTADASTRA_AUTH_MIDDLEWARE_HPP

#include <boost/beast.hpp>
#include <string>
#include <boost/beast/http.hpp>
#include "JWT.hpp"

namespace Softadastra
{

    namespace http = boost::beast::http;

    /**
     * @class AuthMiddleware
     * @brief Middleware class for verifying JWT tokens in HTTP requests.
     *
     * This class provides methods to verify JWT tokens included in the
     * "Authorization" header of HTTP requests. It checks for the presence
     * of the header, extracts the token, and verifies its validity using
     * a provided secret key.
     */
    class AuthMiddleware
    {
    public:
        /**
         * @brief Verifies the JWT token in the HTTP request.
         *
         * This method looks for the "Authorization" header in the request,
         * extracts the JWT token, and verifies it using the provided secret key.
         *
         * @param request The HTTP request containing the Authorization header.
         * @param secret_key The secret key used to verify the JWT token's signature.
         *
         * @return true if the token is valid, false if the header is missing or the token is invalid.
         */
        static bool verifyJWT(const http::request<http::string_body> &request, const std::string &secret_key);

    private:
        /**
         * @brief Extracts the JWT token from the Authorization header.
         *
         * This method assumes the Authorization header follows the format "Bearer <token>",
         * and it extracts and returns the token part of the header.
         *
         * @param auth_header The Authorization header containing the JWT token in "Bearer <token>" format.
         *
         * @return The JWT token as a string, or an empty string if the header does not contain a valid token.
         */
        static std::string extractToken(const std::string &auth_header);
    };

} // namespace Softadastra

#endif // SOFTADASTRA_AUTH_MIDDLEWARE_HPP
