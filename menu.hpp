#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <iostream>

class Menu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

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
    void switchToPlayerSelection();
    void drawButton(sf::RenderWindow& window, const sf::RectangleShape& button, sf::Text& text, sf::Text& neonText);

public:
    Menu(sf::RenderWindow& window);
    ~Menu() {}

    void handleEvents(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
};

#endif
