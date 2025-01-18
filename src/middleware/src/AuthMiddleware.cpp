#include "AuthMiddleware.hpp"
#include <iostream>

namespace Softadastra
{

    bool AuthMiddleware::verifyJWT(const http::request<http::string_body> &request, const std::string &secret_key)
    {
        if (request.find(http::field::authorization) == request.end())
        {
            std::cerr << "Error: Missing Authorization header." << std::endl;
            return false;
        }

        std::string auth_header = request[http::field::authorization].to_string();
        std::string token = extractToken(auth_header);

        return JWT::verify(token, secret_key);
    }

    std::string AuthMiddleware::extractToken(const std::string &auth_header)
    {
        size_t pos = auth_header.find("Bearer ");
        if (pos != std::string::npos)
        {
            return auth_header.substr(pos + 7); // Return the token part after "Bearer "
        }
        return ""; // Return an empty string if "Bearer " is not found
    }

} // namespace Softadastra
