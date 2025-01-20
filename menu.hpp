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

class Menu
{
public:
    Menu(sf::RenderWindow& window);

    // Gère les événements (clavier, souris, etc.)
    void handleEvents(sf::RenderWindow& window);

    // Dessine (render) l'état actuel du menu
    void render(sf::RenderWindow& window);

    // Getter/Setter pour l’état du Menu
    MenuState getCurrentState() const { return currentState; }
    void      setCurrentState(MenuState state) { currentState = state; }

    // Permettra au Jeu de savoir qui a été sélectionné
    const std::vector<PlayerInfo>& getPlayers() const { return players; }

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
    void removePlayer();    // Retire le dernier joueur (min 1)
    void updatePlayerSpritesPositions(); // Aligne les pions dans le menu

    // Référence sur la fenêtre principale
    sf::RenderWindow& window;

    // État courant (MAIN_MENU, PLAYER_SELECTION, GAME_RUNNING)
    MenuState currentState;

    // Ressources graphiques
    sf::Font     font;
    sf::Texture  backgroundTexture;
    sf::Sprite   backgroundSprite;

    // Boutons du Main Menu
    sf::RectangleShape playAloneButton, playWithOthersButton, quitButton;
    sf::Text playAloneText, playWithOthersText, quitText;
    sf::Text neonTextPlayAlone, neonTextPlayWithOthers, neonTextQuit;

    // Menu "Player Selection"
    sf::CircleShape Xbutton;
    int circlerayon;
    sf::RectangleShape plusButton, minusButton;   // Pour ajouter/retirer un joueur
    sf::Text plusText, minusText, neonPlusText, neonMinusText;

    sf::RectangleShape startButton;
    sf::Text startText, neonTextStart;

    // Sons divers
    sf::SoundBuffer clickBuffer, CriBuffer, diceBuffer,
                    gameboardSoundBuffer1, gameboardSoundBuffer2,
                    gameboardSoundBuffer3, gameboardSoundBuffer4;
    sf::Sound clickSound, cri, dice,
              gameboardSound1, gameboardSound2,
              gameboardSound3, gameboardSound4;

    // Joueurs sélectionnés
    std::vector<PlayerInfo> players;  

    // Texture du pion
    sf::Texture pionTexture;

    // Indique si on est en mode "ordinateur" (play alone) ou "local multi" (play with others)
    bool isPlayerComputer;
};

#endif // MENU_HPP
