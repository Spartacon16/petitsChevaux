cmake_minimum_required(VERSION 3.30)

project(petitsChevaux)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Create the library
add_library(petitsChevaux
    jeu_petits_chevaux.cpp
    joueur_humain.cpp
    joueur_ia.cpp
)

# Create the executable
add_executable(JEU main.cpp)

# Link the library to the executable
target_link_libraries(JEU petitsChevaux)
