#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/version.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <thread>

namespace beast = boost::beast;   // From <boost/beast.hpp>
namespace http = beast::http;     // From <boost/beast/http.hpp>
namespace net = boost::asio;      // From <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // From <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;

struct User
{
    std::string name;
    int age;
};

std::vector<User> users; // Stockage en mémoire des utilisateurs

class UserValidator
{
public:
    static bool is_valid_name(const std::string &name)
    {
        return !name.empty() && name.length() <= 100 &&
               std::all_of(name.begin(), name.end(), [](char ch)
                           { return std::isalpha(ch) || ch == ' '; });
    }

    static bool is_valid_age(int age)
    {
        return age >= 0 && age <= 150;
    }
};

void handle_request(const http::request<http::string_body> &req, http::response<http::string_body> &res)
{
    res.version(req.version());
    res.set(http::field::content_type, "application/json");
    res.set(http::field::server, "RESTful API Server");

    try
    {
        if (req.method() == http::verb::post && req.target() == "/users/create")
        {
            // Extraction et validation des données utilisateur
            auto body = json::parse(req.body());

            if (!body.contains("name") || !body.contains("age"))
            {
                res.result(http::status::bad_request);
                res.body() = R"({"error": "Missing 'name' or 'age' field"})";
                return;
            }

            std::string name = body["name"].get<std::string>();
            int age = body["age"].get<int>();

            if (!UserValidator::is_valid_name(name) || !UserValidator::is_valid_age(age))
            {
                res.result(http::status::bad_request);
                res.body() = R"({"error": "Invalid 'name' or 'age'"})";
                return;
            }

            users.push_back({name, age});

            res.result(http::status::created);
            res.body() = R"({"message": "User created successfully"})";
        }
        else if (req.method() == http::verb::get && req.target() == "/users/all")
        {
            // Retourne tous les utilisateurs
            json response = json::array();

            for (const auto &user : users)
            {
                response.push_back({{"name", user.name}, {"age", user.age}});
            }

            res.result(http::status::ok);
            res.body() = response.dump();
        }
        else
        {
            res.result(http::status::not_found);
            res.body() = R"({"error": "Route not found"})";
        }
    }
    catch (const std::exception &e)
    {
        res.result(http::status::internal_server_error);
        res.body() = json({{"error", e.what()}}).dump();
    }

    res.prepare_payload();
}

void do_session(tcp::socket socket)
{
    try
    {
        beast::flat_buffer buffer;
        http::request<http::string_body> req;

        // Lire la requête HTTP
        http::read(socket, buffer, req);

        // Créer la réponse
        http::response<http::string_body> res;
        handle_request(req, res);

        // Envoyer la réponse HTTP
        http::write(socket, res);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void do_accept(net::io_context &ioc, tcp::acceptor &acceptor)
{
    while (true)
    {
        tcp::socket socket(ioc);
        acceptor.accept(socket);

        std::thread([socket = std::move(socket)]() mutable
                    { do_session(std::move(socket)); })
            .detach();
    }
}

int main()
{
    try
    {
        net::io_context ioc{1};

        // Démarrage du serveur sur le port 8080
        tcp::acceptor acceptor{ioc, {net::ip::make_address("0.0.0.0"), 8080}};
        std::cout << "Server running on port 8080..." << std::endl;

        do_accept(ioc, acceptor);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
