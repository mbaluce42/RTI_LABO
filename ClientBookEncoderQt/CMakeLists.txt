cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(ClientBookEncoder)

# Activer le support de Qt5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)  # Désactivation de l'AUTOUIC car pas de fichier .ui
set(CMAKE_AUTORCC ON)

# Utiliser les bibliothèques Qt5
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

# Ajouter les fichiers source
set(SOURCES
    main.cpp
    mainwindowclientbookencoder.cpp
    moc_mainwindowclientbookencoder.cpp
    ../Librairies/socket.cpp
)

# Ajouter les fichiers d'en-tête
set(HEADERS
    mainwindowclientbookencoder.h
    ui_mainwindowclientbookencoder.h  # Ajouter l'en-tête généré
    ../Librairies/socket.h
)

# Générer automatiquement le code MOC pour les classes qui utilisent Qt
qt5_wrap_cpp(MOC_SOURCES ${HEADERS})

# Créer l'exécutable
add_executable(ClientBookEncoder ${SOURCES} ${MOC_SOURCES})

# Lier les bibliothèques Qt nécessaires
target_link_libraries(ClientBookEncoder Qt5::Core Qt5::Widgets Qt5::Gui)

# Définir le compilateur C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
