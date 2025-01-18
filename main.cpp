#include <boost/asio.hpp>
#include <iostream>
#include "Config.hpp"
#include "Routes.hpp"

int main()
{
    try
    {
        // Charger la configuration
        Config config;
        config.loadConfig();

        // Initialiser le contexte d'entrée/sortie
        boost::asio::io_context ioc;

        // Démarrer le serveur avec les routes et la configuration
        Routes routes(ioc, config);
        routes.startServer();

        // Lancer le serveur
        ioc.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }

    return 0;
}
