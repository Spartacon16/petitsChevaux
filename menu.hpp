#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <iostream>
enum MenuState {
    MAIN_MENU,
    PLAYER_SELECTION
};
class Menu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    // Nouveau état du menu
    MenuState currentState = MAIN_MENU;

    sf::RectangleShape playAloneButton, playWithOthersButton, quitButton;
    sf::Text playAloneText, playWithOthersText, quitText;
    sf::Text neonTextPlayAlone, neonTextPlayWithOthers, neonTextQuit;

    sf::SoundBuffer clickBuffer, CriBuffer, diceBuffer, gameboardSoundBuffer1, gameboardSoundBuffer2, gameboardSoundBuffer3, gameboardSoundBuffer4;
    sf::Sound clickSound, cri, dice, gameboardSound1, gameboardSound2, gameboardSound3, gameboardSound4;

    bool isPlayerSelectionActive = false;
    bool isPlayerComputer = true;
    const int maxPlayers = 4;
    std::vector<sf::RectangleShape> playerButtons;
    std::vector<sf::Text> playerNames;

    void loadSound(const std::string& soundPath);
    void drawButtonMenu(sf::RenderWindow& window, const sf::RectangleShape& button, sf::Text& text, sf::Text& neonText);
    void handleMainMenuEvents(sf::Event& event, sf::RenderWindow& window);
    void handlePlayerSelectionEvents(sf::Event& event, sf::RenderWindow& window);
    void renderMainMenu(sf::RenderWindow& window);
    void renderPlayerSelection(sf::RenderWindow& window);
    void handleResize(unsigned int newWidth, unsigned int newHeight); 

public:
    Menu(sf::RenderWindow& window);

    void handleEvents(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
};

#endif //MENU_HPP
