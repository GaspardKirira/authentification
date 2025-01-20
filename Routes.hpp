#ifndef ROUTES_HPP
#define ROUTES_HPP

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/beast/http.hpp>
#include <iostream>
#include <string>
#include <map>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include "Config.hpp"

namespace http = boost::beast::http;
namespace net = boost::asio;

class Routes
{
public:
    Routes(net::io_context &ioc, Config &config)
        : acceptor_(ioc, net::ip::tcp::endpoint(net::ip::make_address("0.0.0.0"), config.getServerPort())), config_(config)
    {
        std::cout << "Serveur démarré sur le port " << config_.getServerPort() << std::endl;
    }

    void startServer()
    {
        doAccept();
    }

private:
    void doAccept()
    {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, net::ip::tcp::socket socket)
            {
                if (!ec)
                {
                    // Créer une session de traitement de la requête HTTP
                    std::make_shared<HttpSession>(std::move(socket), config_)->start();
                }
                doAccept();
            });
    }

    class HttpSession : public std::enable_shared_from_this<HttpSession>
    {
    public:
        explicit HttpSession(net::ip::tcp::socket socket, Config &config)
            : socket_(std::move(socket)), config_(config) {}

        void start()
        {
            doRead();
        }

    private:
        void doRead()
        {
            http::async_read(socket_, buffer_, request_,
                             [self = shared_from_this()](boost::system::error_code ec, std::size_t)
                             {
                                 if (!ec)
                                 {
                                     self->handleRequest();
                                 }
                             });
        }

        void handleRequest()
        {
            // Pour cet exemple, nous allons interagir avec la base de données
            sql::Connection *con = config_.getDbConnection();
            if (con)
            {

                sql::Statement *stmt = con->createStatement();

                stmt->execute("CREATE TABLE IF NOT EXISTS users (id INT PRIMARY KEY AUTO_INCREMENT, username VARCHAR(100));");

                sql::ResultSet *res = stmt->executeQuery("SELECT * FROM users LIMIT 1");

                std::string response_body = "User info:\n";
                while (res->next())
                {
                    response_body += "ID: " + std::to_string(res->getInt("id")) + "\n";
                    response_body += "Username: " + res->getString("username") + "\n";
                }

                delete res;
                delete stmt;
                delete con;

                http::response<http::string_body> response{http::status::ok, request_.version()};
                response.set(http::field::content_type, "text/plain");
                response.body() = response_body;
                response.prepare_payload();

                // Envoyer la réponse
                http::async_write(socket_, response,
                                  [self = shared_from_this()](boost::system::error_code ec, std::size_t)
                                  {
                                      if (ec)
                                      {
                                          std::cerr << "Erreur lors de l'écriture de la réponse: " << ec.message() << std::endl;
                                      }
                                  });
            }
        }

        net::ip::tcp::socket socket_;
        boost::beast::flat_buffer buffer_;
        http::request<http::string_body> request_;
        Config &config_; // Référence à la configuration
    };

    net::ip::tcp::acceptor acceptor_;
    Config &config_; // Référence à la configuration
};

#endif // Routes