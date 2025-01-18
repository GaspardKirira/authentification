cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(MyProject)

# Spécifier la version de C++ à utiliser
set(CMAKE_CXX_STANDARD 20)

# Définir les répertoires d'inclusion
include_directories(/usr/include)

# Définir les fichiers sources
set(SOURCES
    main.cpp
    ErrorHandler.cpp
)

# Définir les options de compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -g -fsanitize=address")

# Définir les options du linker
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

# Créer l'exécutable
add_executable(prog ${SOURCES})

# Lier les bibliothèques nécessaires
target_link_libraries(prog
    boost_system
    pthread
)

# Définir les répertoires de bibliothèque
link_directories(/usr/lib)
