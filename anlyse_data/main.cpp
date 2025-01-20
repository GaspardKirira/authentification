#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#include <string>
#include <json/json.h>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main()
{
    try
    {
        // Données pour le graphique
        std::vector<double> times;
        std::vector<double> prices;

        // Initialisation de curlpp
        auto cleanup = std::make_unique<curlpp::Cleanup>();
        auto response = std::make_unique<std::ostringstream>();
        auto request = std::make_unique<curlpp::Easy>();

        // URL de l'API CoinGecko pour obtenir les prix du Bitcoin sur 24 heures
        request->setOpt<curlpp::options::Url>("https://api.coingecko.com/api/v3/coins/bitcoin/market_chart?vs_currency=usd&days=1");

        // Définir l'agent utilisateur
        request->setOpt<curlpp::options::UserAgent>("curl/7.68.0 GNU/Linux Ubuntu x86_64");

        // Flux pour écrire la réponse
        request->setOpt<curlpp::options::WriteStream>(response.get());

        // Effectuer la requête
        request->perform();

        // Analyser la réponse JSON
        Json::Reader reader;
        Json::Value jsonResponse;
        if (reader.parse(response->str(), jsonResponse))
        {
            // Récupérer les prix et timestamps
            const Json::Value &pricesData = jsonResponse["prices"];
            for (int i = 0; i < pricesData.size(); ++i)
            {
                double timestamp = pricesData[i][0].asDouble(); // timestamp en millisecondes
                double price = pricesData[i][1].asDouble();     // prix en USD

                // Convertir le timestamp en secondes pour l'affichage
                times.push_back(timestamp / 1000.0); // convertir en secondes
                prices.push_back(price);

                // Afficher les données reçues
                std::cout << "Temps: " << times.back() << " - Prix du Softadastra Coin: " << price << " USD" << std::endl;
            }

            // Afficher le graphique avec les nouvelles données
            plt::plot(times, prices);
            plt::title("Prix du Softadastra en temps réel (BTC/USD)");
            plt::xlabel("Temps (en secondes)");
            plt::ylabel("Prix (USD)");

            // Afficher le graphique pendant 5 minutes
            plt::pause(300); // Attendre 300 secondes = 5 minutes
        }
    }
    catch (const curlpp::RuntimeError &e)
    {
        std::cerr << "Erreur d'exécution : " << e.what() << '\n';
    }
    catch (const curlpp::LogicError &e)
    {
        std::cerr << "Erreur logique : " << e.what() << '\n';
    }

    return 0;
}
