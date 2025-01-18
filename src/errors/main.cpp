#include <iostream>
#include <boost/system/error_code.hpp>
#include <boost/beast/http.hpp>
#include "ErrorHandler.hpp"

namespace Softadastra
{
    // Pour simplifier, utilisons un alias pour la réponse HTTP
    using http_response = boost::beast::http::response<boost::beast::http::string_body>;
}

void testErrorHandler(const boost::system::error_code &ec, Softadastra::http_response &response)
{
    // Appel de la méthode de gestion des erreurs en fonction du type d'erreur
    Softadastra::ErrorHandler::handleError(ec, response);
    std::cout << "Code de statut : " << response.result() << std::endl;
    std::cout << "Message de réponse : " << response.body() << std::endl;
}

int main()
{
    // Création de l'objet de réponse HTTP
    Softadastra::http_response response;

    // Simuler différentes erreurs

    // Test pour une erreur de connexion refusée
    std::cout << "Test pour connexion refusée (EC = connection_refused):" << std::endl;
    boost::system::error_code ec1(boost::asio::error::connection_refused);
    testErrorHandler(ec1, response);
    std::cout << std::endl;

    // Test pour une erreur EOF (fin de fichier)
    std::cout << "Test pour EOF (EC = eof):" << std::endl;
    boost::system::error_code ec2(boost::asio::error::eof);
    testErrorHandler(ec2, response);
    std::cout << std::endl;

    // Test pour une erreur de délai d'attente dépassé
    std::cout << "Test pour délai d'attente dépassé (EC = timed_out):" << std::endl;
    boost::system::error_code ec3(boost::asio::error::timed_out);
    testErrorHandler(ec3, response);
    std::cout << std::endl;

    // Test pour une erreur générique
    std::cout << "Test pour une erreur générique (EC = inconnu):" << std::endl;
    boost::system::error_code ec4(12345, boost::system::system_category()); // Erreur générique
    testErrorHandler(ec4, response);
    std::cout << std::endl;

    // Test pour une erreur d'accès non autorisé
    std::cout << "Test pour accès non autorisé:" << std::endl;
    Softadastra::ErrorHandler::handleUnauthorized(response);
    std::cout << "Code de statut : " << response.result() << std::endl;
    std::cout << "Message de réponse : " << response.body() << std::endl;
    std::cout << std::endl;

    // Test pour une erreur d'accès interdit
    std::cout << "Test pour accès interdit:" << std::endl;
    Softadastra::ErrorHandler::handleForbidden(response);
    std::cout << "Code de statut : " << response.result() << std::endl;
    std::cout << "Message de réponse : " << response.body() << std::endl;

    return 0;
}
