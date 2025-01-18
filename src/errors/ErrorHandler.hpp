#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include <boost/beast.hpp>
#include <boost/system/error_code.hpp>
#include <string>

namespace Softadastra
{
    namespace http = boost::beast::http;

    /**
     * @class ErrorHandler
     * @brief A utility class to handle errors and generate HTTP responses accordingly.
     *
     * This class provides methods to handle different types of errors (e.g., connection errors,
     * timeout errors) and respond with appropriate HTTP status codes and error messages.
     */
    class ErrorHandler
    {
    public:
        /**
         * @brief Handles errors and sets the appropriate HTTP status code and message.
         *
         * This method examines the error code and adjusts the HTTP response status and message.
         * Common error codes like connection refusal, timeouts, and malformed requests are handled.
         * A generic error response is provided for other unhandled errors.
         *
         * @param ec The error code that indicates the type of error.
         * @param response The HTTP response object that will be updated with the error details.
         */
        static void handleError(const boost::system::error_code &ec, http::response<http::string_body> &response);

        /**
         * @brief Handles unauthorized access errors.
         *
         * This method sets the HTTP response status to 401 Unauthorized and provides an appropriate
         * error message for unauthorized access.
         *
         * @param response The HTTP response object to be updated.
         */
        static void handleUnauthorized(http::response<http::string_body> &response);

        /**
         * @brief Handles forbidden access errors.
         *
         * This method sets the HTTP response status to 403 Forbidden and provides an appropriate
         * error message for forbidden access.
         *
         * @param response The HTTP response object to be updated.
         */
        static void handleForbidden(http::response<http::string_body> &response);
    };
} // namespace Softadastra

#endif // ERROR_HANDLER_HPP
