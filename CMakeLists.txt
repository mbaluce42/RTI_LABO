cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(ServeurClient)

# Définir le compilateur C++
set(CMAKE_CXX_STANDARD 11)

# Ajouter les options de débogage
add_definitions(-DDEBUG)

# Ajouter l'inclusion de la bibliothèque MySQL
include_directories(/usr/include/mysql)

# Ajouter les sources pour les librairies
set(LIB_SOURCES
    Librairies/socket.cpp
    Protocole/Prot_OBEP.cpp
)

# Créer une librairie statique (ou partagée si besoin)
add_library(libsocket STATIC Librairies/socket.cpp)
add_library(libobep STATIC Protocole/Prot_OBEP.cpp)

# Cibles pour le serveur
add_executable(serveur serveur.cpp)
target_link_libraries(serveur libsocket libobep pthread)  # Ajout de pthread ici

# Cibles pour le client
add_executable(client client.cpp)
target_link_libraries(client libsocket)
