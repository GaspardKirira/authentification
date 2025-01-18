#include "Hash.hpp"
#include <bcrypt.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <iostream>
#include <openssl/evp.h>

namespace Softadastra
{
    std::string Hash::hashPassword(const std::string &password)
    {
        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int length = 0;

        // Créer un contexte EVP pour SHA-256
        EVP_MD_CTX *ctx = EVP_MD_CTX_new();
        if (ctx == nullptr)
        {
            throw std::runtime_error("Erreur d'allocation du contexte EVP.");
        }

        // Initialiser le contexte pour SHA-256
        if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1)
        {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("Erreur lors de l'initialisation de SHA-256.");
        }

        // Mettre à jour le contexte avec le mot de passe
        if (EVP_DigestUpdate(ctx, password.c_str(), password.length()) != 1)
        {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("Erreur lors de la mise à jour de SHA-256.");
        }

        // Finaliser le calcul du hash
        if (EVP_DigestFinal_ex(ctx, hash, &length) != 1)
        {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("Erreur lors de la finalisation du hash SHA-256.");
        }

        // Convertir le hash en une chaîne hexadécimale
        std::stringstream ss;
        for (unsigned int i = 0; i < length; i++)
        {
            ss << std::setw(2) << std::setfill('0') << std::hex << (int)hash[i];
        }

        EVP_MD_CTX_free(ctx);
        return ss.str();
    }

    // Comparaison du mot de passe avec un hash existant (utilisant bcrypt)
    bool Hash::comparePassword(const std::string &password, const std::string &stored_hash)
    {
        if (bcrypt_checkpw(password.c_str(), stored_hash.c_str()) == 0)
        {
            return true; // Le mot de passe correspond au hash
        }
        else
        {
            return false; // Le mot de passe ne correspond pas au hash
        }
    }
}
