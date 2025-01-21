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

struct Case {
    sf::Vector2f position; // Position graphique en pixels
    std::string type;      // Type de case : "Prison", "Parcours", "Finale", "Victoire"
    int playerId;          // ID du joueur (0-3 pour 4 joueurs), -1 si neutre
    int finalStep;         // Étape finale (1-6), 0 si non applicable

    Case(sf::Vector2f pos, const std::string& t, int id = -1, int step = 0)
        : position(pos), type(t), playerId(id), finalStep(step) {}
};
class Jeu {
public:
    Jeu(sf::RenderWindow& window,const sf::Font& font);

    // Définir les joueurs sélectionnés
    void setPlayers(const std::vector<PlayerInfo>& playersSelected);

    // Lancer la boucle de jeu
    void run(sf::RenderWindow& window);

    // Obtenir la taille et la couleur des pions pour le menu
    float getPionScale() const;
    const std::vector<sf::Color>& getPlayerColors() const;

    // Gestion du dé
    void lancerDe();

    // Passer au joueur suivant
    void passerAuJoueurSuivant();

private:
    // Gestion des événements
    void handleEvents(sf::RenderWindow& window);

    // Mettre à jour la logique du jeu
    void update();

    // Rendu graphique
    void render(sf::RenderWindow& window);

    // Configurer le plateau
    void setupPlateau();
    // Générer les cases de parcours
    void generateParcours();
    // Obtenir la position d'une case (placeholder pour un déplacement futur)
    sf::Vector2f getCasePosition(int caseIndex, int playerStartIndex);

    sf::RenderWindow& window;

    // État du jeu
    bool isRunning;

    // Liste de joueurs sélectionnés
    std::vector<PlayerInfo> playersInGame;

    // Liste des pions par joueur
    std::vector<std::vector<PionInfo>> playerPions;
    int joueurActuel; // Index du joueur actuel
    // Variables pour gérer le dé
    int valeurDe;
    sf::RectangleShape boutonLancerDe;
    sf::Text texteLancerDe;
    sf::Text texteActions;
    // Font pour le texte
    sf::Font font;
    // Textures pour le plateau et les pions
    sf::Texture plateauTexture;
    sf::Sprite plateauSprite;

    sf::Texture pionTexture;

    // Échelle des pions pour les ajuster à la taille d'un carreau
    float pionScale;

    // Positions de départ pour chaque joueur
    std::vector<sf::Vector2f> startingPositions;

    std::vector<Case> cases; // Liste de toutes les cases du plateau
    std::map<int, std::vector<sf::Vector2f>> prisonPositions; // Pions de départ
    // Couleurs pastel des joueurs
    std::vector<sf::Color> playerColors;
};

#endif
