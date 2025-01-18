# Spécifiez la version minimale de CMake requise
cmake_minimum_required(VERSION 3.10)

# Définir le nom du projet
project(Softadastra)

# Définir la version de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Définir les options de compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -g -fsanitize=address")

# Définir les options du linker
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

# Ajouter les fichiers sources
# Spécifiez le chemin du dossier "src"
file(GLOB SOURCES "src/*.cpp")

# Ajouter l'exécutable
add_executable(prog ${SOURCES})

# Lier les bibliothèques nécessaires (jwt, ssl et crypto)
find_package(OpenSSL REQUIRED)
target_link_libraries(prog jwt OpenSSL::SSL OpenSSL::Crypto)
