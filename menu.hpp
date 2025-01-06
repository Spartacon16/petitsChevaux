#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

class Menu {
public:
    Menu(sf::RenderWindow& window);
    void handleEvents(sf::RenderWindow& window);  // Gérer les événements
    void render(sf::RenderWindow& window);  // Afficher les éléments
    void update();  // Mettre à jour les éléments (si nécessaire)
    void switchToPlayerSelection();  // Passer au menu de sélection des joueurs
    void startGame();  // Démarrer le jeu
    void drawButton(sf::RenderWindow& window,const sf::RectangleShape& button, sf::Text& text, sf::Text& neonText);
    bool isPlayerSelectionActive;  // Indique si le menu de sélection des joueurs est actif

    sf::RenderWindow window;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

private:
    // Boutons
    sf::RectangleShape playAloneButton;
    sf::Text playAloneText;
     sf::Text neonTextPlayAlone;

    sf::RectangleShape playWithOthersButton;
    sf::Text playWithOthersText;
     sf::Text neonTextPlayWithOthers;

    sf::RectangleShape quitButton;
    sf::Text quitText;
    sf::Text neonTextQuit;

};

// class PlayerSelectionMenu  {
// public:
//     PlayerSelectionMenu();
//     void handleEvents(sf::RenderWindow& window);  // Gérer les événements
//     void render(sf::RenderWindow& window);  // Afficher les éléments
//     void update();  // Mettre à jour les éléments
//     void handleColorSelection(int playerIndex);  // Gérer la sélection des couleurs
//     void selectPlayerColor(int playerIndex, const sf::Color& color);  // Sélectionner une couleur pour un joueur

//     sf::RenderWindow window;
//     sf::Font font;
//     sf::Texture backgroundTexture;
//     sf::Sprite backgroundSprite;

// private:
//     std::vector<sf::RectangleShape> playerButtons;  // Boutons pour les joueurs
//     std::vector<sf::Text> playerText;  // Texte pour chaque joueur
//     std::vector<sf::Color> playerColors;  // Couleurs des joueurs
//     int playerCount;  // Nombre de joueurs sélectionnés

//     // Boutons + et -
//     sf::RectangleShape addButton;
//     sf::RectangleShape removeButton;

//     // Texte des boutons
//     sf::Text addButtonText;
//     sf::Text removeButtonText;
// };

#endif // MENU_HPP
