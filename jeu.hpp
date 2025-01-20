#ifndef JEU_HPP
#define JEU_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "menu.hpp" // pour utiliser PlayerInfo

// Structure pour un pion
struct PionInfo {
    sf::Sprite sprite;       // Sprite pour chaque pion
    sf::Vector2f startPosition; // Position de départ
};

class Jeu {
public:
    Jeu(sf::RenderWindow& window);

    // Définir les joueurs sélectionnés
    void setPlayers(const std::vector<PlayerInfo>& playersSelected);

    // Lancer la boucle de jeu
    void run(sf::RenderWindow& window);

    // Obtenir la taille et la couleur des pions pour le menu
    float getPionScale() const;
    const std::vector<sf::Color>& getPlayerColors() const;

private:
    // Gestion des événements
    void handleEvents(sf::RenderWindow& window);

    // Mettre à jour la logique du jeu
    void update();

    // Rendu graphique
    void render(sf::RenderWindow& window);

    // Configurer les pions pour chaque joueur
    void setupPions();

    // Obtenir la position d'une case (placeholder pour un déplacement futur)
    sf::Vector2f getCasePosition(int caseIndex, int playerStartIndex);

private:
    sf::RenderWindow& window;

    // État du jeu
    bool isRunning;

    // Liste de joueurs sélectionnés
    std::vector<PlayerInfo> playersInGame;

    // Liste des pions par joueur
    std::vector<std::vector<PionInfo>> playerPions;

    // Textures pour le plateau et les pions
    sf::Texture plateauTexture;
    sf::Sprite plateauSprite;

    sf::Texture pionTexture;

    // Échelle des pions pour les ajuster à la taille d'un carreau
    float pionScale;

    // Positions de départ pour chaque joueur
    std::vector<sf::Vector2f> startingPositions;

    // Couleurs pastel des joueurs
    std::vector<sf::Color> playerColors;
};

#endif
