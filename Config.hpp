#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <iostream>
#include <fstream>
#include <json/json.h> 

class Config
{
public:
    Config() : db_host("localhost"), db_user("root"), db_pass("password"), db_name("mydatabase"), server_port(8080) {}

    void loadConfig()
    {
        std::ifstream config_file("config.json");
        if (!config_file.is_open())
        {
            std::cerr << "Erreur lors de l'ouverture du fichier de configuration." << std::endl;
            return;
        }

        Json::Value config;
        config_file >> config;

        // Charger la configuration à partir du fichier JSON
        db_host = config["database"]["host"].asString();
        db_user = config["database"]["user"].asString();
        db_pass = config["database"]["password"].asString();
        db_name = config["database"]["name"].asString();
        server_port = config["server"]["port"].asInt();

        std::cout << "Configuration chargée : " << std::endl;
        std::cout << "Host: " << db_host << ", Port: " << server_port << std::endl;
    }

    // Connexion à la base de données MySQL
    sql::Connection *getDbConnection()
    {
        try
        {
            sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
            sql::Connection *con = driver->connect("tcp://" + db_host + ":3306", db_user, db_pass);
            con->setSchema(db_name);
            return con;
        }
        catch (sql::SQLException &e)
        {
            std::cerr << "Erreur de connexion à la base de données: " << e.what() << std::endl;
            return nullptr;
        }
    }

    // Getters pour la configuration
    const std::string &getDbHost() const { return db_host; }
    const std::string &getDbUser() const { return db_user; }
    const std::string &getDbPass() const { return db_pass; }
    const std::string &getDbName() const { return db_name; }
    int getServerPort() const { return server_port; }

private:
    std::string db_host;
    std::string db_user;
    std::string db_pass;
    std::string db_name;
    int server_port;
};

#endif // CONFIG_HPP
