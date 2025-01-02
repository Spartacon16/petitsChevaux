#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "graphique.hpp"  
#include "jeu.hpp" 

class Menu {
public:
    Menu();
    void run();

private:
    void handleEvents();
    void render();
    void startGame();  // Méthode pour démarrer le jeu

    sf::RenderWindow window;

    // Variables pour les boutons
    sf::RectangleShape playAloneButton;
    sf::Text playAloneText;
    sf::RectangleShape playWithOthersButton;
    sf::Text playWithOthersText;
    sf::RectangleShape quitButton;
    sf::Text quitText;

    // Variables pour le fond
    sf::Texture backgroundTexture;  // Déclare la texture pour l'image de fond
    sf::Sprite backgroundSprite;    // Déclare le sprite pour afficher la texture

    // Police
    sf::Font font;
};

#endif
