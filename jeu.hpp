#ifndef JEU_HPP
#define JEU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <map>
#include "menu.hpp" // pour utiliser PlayerInfo

// Structure pour un pion
struct PionInfo {
    sf::Sprite sprite;        // Sprite pour chaque pion
    sf::Vector2f startPosition; // Position de départ
    bool isOut;               // Indique si le pion est sorti de la prison
    int pionId;   // Id de la couleur du pion de jeu -1 si neutre, // couleur de la case de jeu -1 si neutre, 0"Yellow",3"Blue",1"Red",2"Green"
   
};

// Structure pour une case
struct Case {
    sf::Vector2f position;    // Position graphique en pixels
    std::string type;         // Type de case : "Prison", "Parcours", "Finale", "Victoire","AvantFinal"
    int caseId;   // couleur de la case de jeu -1 si neutre, // couleur de la case de jeu -1 si neutre, 0"Yellow",3"Blue",1"Red",2"Green"
    int finalStep;            // Étape finale (1-6), 0 si non applicable, 7 si gagné

    Case(sf::Vector2f pos, const std::string& t, int id = -1, int step = 0)
        : position(pos), type(t), caseId(id), finalStep(step) {}
};

class Jeu {
public:
    Jeu(sf::RenderWindow& window, const sf::Font& font,const int& nmbpion,
        const sf::Sound& clickSound, const sf::Sound& criSound,
        const sf::Sound& diceSound, const std::vector<sf::Sound>& gameboardSounds);
                                   // Générer les cases de parcours
    sf::Vector2f getCasePosition(int caseIndex, int playerStartIndex); // Obtenir la position d'une case
    int trouverIndexCase(const sf::Vector2f& position);              // Trouver l'index d'une case
    void setPlayers(const std::vector<PlayerInfo>& playersSelected); // Définir les joueurs sélectionnés
    void run(sf::RenderWindow& window);   
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
private:                         
    void lancerDe() ;
    void handleEvents(sf::RenderWindow& window);                     // Gestion des événements
    void update();                                                   // Mettre à jour la logique du jeu
    void render(sf::RenderWindow& window);                           // Rendu graphique
    void setupPlateau();                                             // Configurer le plateau
    void generateParcours();      void gererClicPion(const sf::Vector2f& mousePos);                // Gérer le clic sur un pion
    bool sortirPion(PionInfo& pion);                                 // Sortir un pion
    bool avancerPion(PionInfo& pion);                                // Avancer un pion
    void passerAuJoueurSuivant();                                    // Passer au joueur suivant
    bool peutAvancerPion(PionInfo& pion);
    sf::RenderWindow& window; // Fenêtre de rendu
    bool isRunning;           // État du jeu
    int joueurActuel;         // Index du joueur actuel
    int valeurDe;             // Valeur actuelle du dé
    float pionScale;           // Échelle des pions
    bool diceRolled;          // Indique si le dé a été lancé
    bool attenteValidation; // Indique si on attend une validation (bouton "OK")
    int nmbpion;            // Indique le nombres de pion dans le jeu
    // Textures et éléments graphiques
    sf::Texture plateauTexture;
    sf::Sprite plateauSprite;
    sf::Texture pionTexture;
    sf::RectangleShape boutonLancerDe;
    sf::Text texteLancerDe;
    sf::Text texteActions;
    sf::Font font;
    sf::Sprite txt_pionJoueurActuel;

    // Gestion des joueurs et pions
    std::vector<PlayerInfo> playersInGame;           // Joueurs sélectionnés
    std::vector<std::vector<PionInfo>> playerPions;  // Pions par joueur
    std::vector<sf::Color> playerColors;             // Couleurs des joueurs
    std::vector<sf::Vector2f> startingPositions;     // Positions de départ des pions

    // Plateau et cases
    std::vector<Case> cases;                         // Liste de toutes les cases du plateau
    std::map<int, std::vector<sf::Vector2f>> prisonPositions; // Positions des prisons

    // Sons
    sf::Sound clickSound;
    sf::Sound criSound;
    sf::Sound diceSound;
    std::vector<sf::Sound> gameboardSounds;
};

#endif
