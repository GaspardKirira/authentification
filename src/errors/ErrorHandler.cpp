#include "ErrorHandler.hpp"
#include <boost/beast/http.hpp>
#include <boost/system/error_code.hpp>

namespace Softadastra
{
    void ErrorHandler::handleError(const boost::system::error_code &ec, boost::beast::http::response<boost::beast::http::string_body> &response)
    {
        if (ec == boost::asio::error::connection_refused)
        {
            response.result(boost::beast::http::status::service_unavailable);
            response.body() = "Service indisponible. Veuillez réessayer plus tard.";
        }
        else if (ec == boost::asio::error::eof)
        {
            response.result(boost::beast::http::status::bad_request);
            response.body() = "Requête malformée.";
        }
        else if (ec == boost::asio::error::timed_out)
        {
            response.result(boost::beast::http::status::request_timeout);
            response.body() = "Délai d'attente dépassé.";
        }
        else
        {
            response.result(boost::beast::http::status::internal_server_error);
            response.body() = "Une erreur interne est survenue.";
        }

        response.set(boost::beast::http::field::content_type, "text/plain");
        response.prepare_payload();
    }

    void ErrorHandler::handleUnauthorized(boost::beast::http::response<boost::beast::http::string_body> &response)
    {
        response.result(boost::beast::http::status::unauthorized);
        response.body() = "Accès non autorisé. Veuillez fournir un token valide.";
        response.set(boost::beast::http::field::content_type, "text/plain");
        response.prepare_payload();
    }

    void ErrorHandler::handleForbidden(boost::beast::http::response<boost::beast::http::string_body> &response)
    {
        response.result(boost::beast::http::status::forbidden);
        response.body() = "Accès interdit.";
        response.set(boost::beast::http::field::content_type, "text/plain");
        response.prepare_payload();
    }
}
