#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <thread>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <unordered_map>

namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace net = boost::asio;

using tcp = net::ip::tcp;
using json = nlohmann::json;

struct User
{
    std::string m_name{};
    std::vector<std::string> m_skills{};

    User() {}

    User(const std::string &name, const std::vector<std::string> &skills)
        : m_name(name), m_skills(skills) {}

    User(const User &rhs)
        : m_name(rhs.m_name), m_skills(rhs.m_skills) {}

    User &operator=(const User &rhs)
    {
        if (this != &rhs)
        {
            m_name = rhs.m_name;
            m_skills = rhs.m_skills;
        }
        return *this;
    }

    User(User &&) = default;
    User &operator=(User &&) = default;
    ~User() = default;

    json to_json() const
    {
        return json{{"name", m_name}, {"skills", m_skills}};
    }

    void from_json(json &j)
    {
        j.at("name").get_to(m_name);
        j.at("skills").get_to(m_skills);
    }

    const std::string &get_name() const { return m_name; }
    const std::vector<std::string> &get_skills() const { return m_skills; }
};

struct UserValidator
{
    static bool is_valid_name(const std::string &name)
    {
        return !name.empty() && name.length() <= 100 &&
               std::all_of(name.begin(), name.end(), [](char ch)
                           { return std::isalpha(ch) || std::isspace(ch) || ch == '\'' || ch == '-'; });
    }

    static bool is_valid_skills(const std::vector<std::string> &skills)
    {
        // Vérifier que le vecteur n'est pas vide
        if (skills.empty())
        {
            std::cout << "Skills list is empty." << std::endl;
            return false;
        }

        // Vérifier que chaque compétence respecte les règles (par exemple, longueur max et non vide)
        for (const auto &skill : skills)
        {
            if (skill.empty())
            {
                std::cout << "Skill is empty: " << skill << std::endl;
                return false;
            }
            if (skill.length() > 50)
            {
                std::cout << "Skill too long: " << skill << std::endl;
                return false;
            }
            if (!std::all_of(skill.begin(), skill.end(), [](char ch)
                             { return std::isprint(ch); }))
            {
                std::cout << "Skill contains invalid characters: " << skill << std::endl;
                return false;
            }
        }

        // Vérification des compétences uniques
        std::vector<std::string> unique_skills = skills;
        std::sort(unique_skills.begin(), unique_skills.end());
        auto last = std::unique(unique_skills.begin(), unique_skills.end());
        unique_skills.erase(last, unique_skills.end());

        if (unique_skills.size() != skills.size())
        {
            std::cout << "Duplicate skills found." << std::endl;
            return false; // Des compétences en double ont été trouvées
        }

        return true;
    }
};

struct UserService
{
    std::vector<User> users{};

    UserService(std::initializer_list<User> user_list) : users(user_list) {}
    ~UserService() {}

    void add(const User &user)
    {
        // Vérifiez si l'utilisateur existe déjà
        for (const auto &existing_user : users)
        {
            if (existing_user.get_name() == user.get_name())
            {
                // Si l'utilisateur existe déjà, ne l'ajoutez pas
                std::cout << "User already exists!" << std::endl;
                return; // Ou vous pouvez gérer l'erreur autrement
            }
        }

        // Si l'utilisateur n'existe pas, ajoutez-le
        users.push_back(user);
    }

    void remove()
    {
        users.pop_back();
    }

    json to_json() const
    {
        json data{};
        if (!users.empty())
        {
            for (const auto &user : users)
            {
                data.emplace_back(user.to_json());
            }
            return data;
        }
        else
        {
            return json::parse(R"({"message": "vector is empty"})");
        }
    }

    std::vector<User> get_all_users() const
    {
        return users;
    }
};

UserService service{};

using RouteHandler = std::function<void(const http::request<http::string_body> &, http::response<http::string_body> &)>;

// Maintenant, dans le map
std::unordered_map<std::string, RouteHandler> routes = {
    {"GET /", [](const auto &, auto &res)
     {
         res.result(http::status::ok);
         res.set(http::field::content_type, "application/json");
         res.body() = json{{"message", "Bienvenu sur la page d'accueil"}}.dump();
     }},
    {"GET /posts", [](const auto &, auto &res)
     {
         res.result(http::status::ok);
         res.set(http::field::content_type, "application/json");
         res.body() = service.to_json().dump();
     }},
    {"POST /create", [](const auto &req, auto &res)
     {
         try
         {
             auto json_data = json::parse(req.body());
             if (json_data.find("name") == json_data.end())
             {
                 res.result(http::status::bad_request);
                 res.body() = json{{"message", "Missing 'name' field"}}.dump();
                 return;
             }

             std::string name = json_data["name"];

             if (!UserValidator::is_valid_name(name))
             {
                 res.result(http::status::bad_request);
                 res.set(http::field::content_type, "application/json");
                 res.body() = json{{"message", "Invalid name"}}.dump();
                 return;
             }

             if (json_data.find("skills") == json_data.end() || !json_data["skills"].is_array())
             {
                 res.result(http::status::bad_request);
                 res.set(http::field::content_type, "application/json");
                 res.body() = json{{"message", "skills should be an array"}}.dump();
                 return;
             }

             std::vector<std::string> skills = json_data["skills"].template get<std::vector<std::string>>();

             if (!UserValidator::is_valid_skills(skills))
             {
                 res.result(http::status::bad_request);
                 res.set(http::field::content_type, "application/json");
                 res.body() = json{{"message", "Invalid skills"}}.dump();
                 return;
             }

             User user(name, skills);

             bool user_exist = false;

             for (const auto &existing_user : service.get_all_users())
             {
                 if (existing_user.get_name() == user.get_name())
                 {
                     user_exist = true;
                     break;
                 }
             }

             if (user_exist)
             {
                 res.result(http::status::bad_request);
                 res.set(http::field::content_type, "application/json");
                 res.body() = json{{"message", "User already exists"}}.dump();
                 return;
             }

             service.add(user);

             res.result(http::status::ok);
             res.set(http::field::content_type, "application/json");
             res.body() = json{{"message", "User created successfully!"}}.dump();
         }
         catch (const std::exception &e)
         {
             res.result(http::status::bad_request);
             res.set(http::field::content_type, "application/json");
             res.body() = json{{"message", std::string("Invalid JSON data: ") + e.what()}}.dump();
         }
     }}

};

void handle_request(const http::request<http::string_body> &req, http::response<http::string_body> &res)
{
    std::string key = std::string(req.method_string()) + " " + std::string(req.target());

    auto it = routes.find(key);
    if (it != routes.end())
    {
        it->second(req, res);
    }
    else
    {
        res.result(http::status::not_found);
        res.set(http::field::content_type, "application/json");
        res.body() = "Route not found";
    }
}

void do_session(tcp::socket socket)
{
    try
    {
        beast::flat_buffer buffer{8024};
        http::request<http::string_body> req;
        http::read(socket, buffer, req);
        http::response<http::string_body> res;
        handle_request(req, res);
        http::write(socket, res);
    }
    catch (const std::exception &e)
    {
        std::cerr << "session error: " << e.what() << std::endl;
    }
}

int main()
{
    try
    {

        net::io_context ioc{};
        tcp::acceptor acceptor{ioc, {tcp::v4(), 8080}};
        std::cout << "Server running on port 8080...." << std::endl;
        while (true)
        {
            tcp::socket socket{ioc};
            acceptor.accept(socket);
            std::thread{do_session, std::move(socket)}.detach();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}