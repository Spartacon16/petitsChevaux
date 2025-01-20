cmake_minimum_required(VERSION 3.31.1)

# Nom du projet
project(petitsChevaux LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2 COMPONENTS system window graphics REQUIRED)

# Fichiers source
add_executable(${PROJECT_NAME} main.cpp menu.cpp jeu.cpp music.cpp)

# Lier SFML
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio)