#include <boost/beast.hpp>
#include <iostream>
#include <string>
#include <jwt-cpp/jwt.h>
#include <boost/beast/http.hpp>
#include "JWT.hpp"
#include "AuthMiddleware.hpp"

int main()
{
    // Clé secrète partagée pour le JWT
    std::string secret_key = "kdjddhijhsdfjibdsjifghsdgiujhsigujhsgsjhsjhlkfnfjfjhfhiusuishnb7373633uuj";

    // Générer un JWT pour un utilisateur fictif (par exemple "user123")
    std::string user_id = "56";
    std::string token = Softadastra::JWT::generate(user_id, secret_key);
    std::cout << "JWT généré : " << token << std::endl;

    // Créer une requête HTTP simulée avec l'en-tête Authorization
    boost::beast::http::request<boost::beast::http::string_body> req;
    req.set(boost::beast::http::field::authorization, "Bearer " + token);

    // Utiliser le middleware pour vérifier le JWT
    if (Softadastra::AuthMiddleware::verifyJWT(req, secret_key))
    {
        std::cout << "Le JWT est valide !" << std::endl;
    }
    else
    {
        std::cout << "Le JWT est invalide." << std::endl;
    }

    return 0;
}
