#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <array>

// État global du menu
enum MenuState {
    MAIN_MENU,
    PLAYER_SELECTION,
    GAME_RUNNING
};

// Informations d'un joueur
struct PlayerInfo {
    std::string name;      // ex: "J1", "COM2", ...
    sf::Color color;       // Couleur associée (rouge, vert, bleu, jaune, etc.)
    sf::Sprite sprite;     // Pour afficher le pion
};

class Menu {
public:
    Menu(sf::RenderWindow& window);

    // Gère les événements (clavier, souris, etc.)
    void handleEvents(sf::RenderWindow& window);

    // Dessine (render) l'état actuel du menu
    void render(sf::RenderWindow& window);

    // Getter/Setter pour l'état du Menu
    MenuState getCurrentState() const { return currentState; }
    void setCurrentState(MenuState state) { currentState = state; }

    // Permettra au Jeu de savoir qui a été sélectionné
    const std::vector<PlayerInfo>& getPlayers() const { return players; }

    // Accesseurs pour les sons
    const sf::Sound& getClickSound() const { return clickSound; }
    const sf::Sound& getCriSound() const { return cri; }
    const sf::Sound& getDiceSound() const { return dice; }
    const std::vector<sf::Sound>& getGameboardSounds() const { return gameboardSounds; }

    // Ressources graphiques
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

private:
    // Fonctions internes
    void loadSound(const std::string& soundPath);
    void loadTextures();
    void handleMainMenuEvents(sf::Event& event, sf::RenderWindow& window);
    void handlePlayerSelectionEvents(sf::Event& event, sf::RenderWindow& window);
    void handleResize(unsigned int newWidth, unsigned int newHeight);

    void renderMainMenu(sf::RenderWindow& window);
    void renderPlayerSelection(sf::RenderWindow& window);
    void drawButtonMenu(sf::RenderWindow& window,
                        const sf::RectangleShape& button,
                        sf::Text& text,
                        sf::Text& neonText);

    // --- Gestion du Player Selection ---
    void initPlayers();     // Initialise 2 joueurs par défaut selon isPlayerComputer
    void addPlayer();       // Ajoute un nouveau joueur (max 4)
    void removePlayer();    // Retire le dernier joueur (min 2)
    void updatePlayerSpritesPositions(); // Aligne les pions dans le menu

    // Référence sur la fenêtre principale
    sf::RenderWindow& window;

    // État courant (MAIN_MENU, PLAYER_SELECTION, GAME_RUNNING)
    MenuState currentState;

    // Boutons du Main Menu
    sf::RectangleShape playAloneButton, playWithOthersButton, quitButton;
    sf::Text playAloneText, playWithOthersText, quitText;
    sf::Text neonTextPlayAlone, neonTextPlayWithOthers, neonTextQuit;

    // Menu "Player Selection"
    sf::CircleShape Xbutton;
    int circlerayon;
    sf::RectangleShape plusButton, minusButton;
    sf::Text plusText, minusText, neonPlusText, neonMinusText;
    sf::RectangleShape startButton;
    sf::Text startText, neonTextStart;

    // Sons divers
    sf::SoundBuffer clickBuffer, criBuffer, diceBuffer;
    sf::Sound clickSound, cri, dice;

    std::vector<sf::SoundBuffer> gameboardSoundBuffers; // Stocke les buffers des sons du plateau
    std::vector<sf::Sound> gameboardSounds; // Stocke les sons associés

    // Joueurs sélectionnés
    std::vector<PlayerInfo> players;

    // Texture du pion
    sf::Texture pionTexture;

    // Indique si on est en mode "ordinateur" (play alone) ou "local multi" (play with others)
    bool isPlayerComputer;
};

#endif // MENU_HPP
