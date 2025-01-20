#include "menu.hpp"

Menu::Menu(sf::RenderWindow& window) : window(window) {
    // Charger la police depuis le dossier petitschevaux
    if (!font.loadFromFile("../BrownieStencil-8O8MJ.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police petitschevaux/arial.ttf" << std::endl;
        exit(-1);
    }

    // Charger l'image de fond depuis le dossier petitschevaux
    if (!backgroundTexture.loadFromFile("../menu_background.jpg")) {
        std::cerr << "Erreur : Impossible de charger l'image de fond petitschevaux/menu_background.webp" << std::endl;
        exit(-1);
    }
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
    );
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(sf::Color(255, 255, 255, 200));

    const sf::Vector2f buttonSize(200, 50);
    const float rightMargin = 70;
    const float buttonSpacing = 20;
    const int fontsize = 26;

    // Initialisation des boutons
    playAloneButton.setSize(buttonSize);
    playAloneButton.setFillColor(sf::Color::Transparent);
    playAloneButton.setPosition(window.getSize().x - buttonSize.x - rightMargin, 150);
    playAloneText.setFont(font);
    playAloneText.setString("Play Alone");
    playAloneText.setCharacterSize(fontsize);
    playAloneText.setStyle(sf::Text::Bold);
    playAloneText.setFillColor(sf::Color::White);
    playAloneText.setPosition(playAloneButton.getPosition().x + 20, playAloneButton.getPosition().y + 10);
    neonTextPlayAlone = playAloneText;
    neonTextPlayAlone.setPosition(playAloneButton.getPosition().x + 18, playAloneButton.getPosition().y + 8);

    playWithOthersButton = playAloneButton;
    playWithOthersButton.setPosition(playAloneButton.getPosition().x, playAloneButton.getPosition().y + buttonSize.y + buttonSpacing);
    playWithOthersText = playAloneText;
    playWithOthersText.setString("Play with Others");
    playWithOthersText.setPosition(playWithOthersButton.getPosition().x + 20, playWithOthersButton.getPosition().y + 10);
    neonTextPlayWithOthers = playWithOthersText;
    neonTextPlayWithOthers.setPosition(playWithOthersButton.getPosition().x + 18, playWithOthersButton.getPosition().y + 8);

    quitButton = playAloneButton;
    quitButton.setPosition(playWithOthersButton.getPosition().x, playWithOthersButton.getPosition().y + buttonSize.y + buttonSpacing);
    quitText = playAloneText;
    quitText.setString("Quit");
    quitText.setPosition(quitButton.getPosition().x + 20, quitButton.getPosition().y + 10);
    neonTextQuit = quitText;
    neonTextQuit.setPosition(quitButton.getPosition().x + 18, quitButton.getPosition().y + 8);

    loadSound("../bruits");

    isPlayerSelectionActive = false;
    isPlayerComputer = true; // Défaut sur ordinateur en solo
}

void Menu::loadSound(const std::string& soundPath) {
    // Chargement des sons
    if (!clickBuffer.loadFromFile(soundPath + "/clic.mp3")) {
        std::cerr << "Erreur : Impossible de charger le fichier son clic.mp3" << std::endl;
    }
    clickSound.setBuffer(clickBuffer);
    clickSound.setVolume(10000);  // Volume réglé à 10000

    if (!CriBuffer.loadFromFile(soundPath + "/cri.mp3")) {
        std::cerr << "Erreur : Impossible de charger le fichier son cri.mp3" << std::endl;
    }
    cri.setBuffer(CriBuffer);

    if (!diceBuffer.loadFromFile(soundPath + "/dice.mp3")) {
        std::cerr << "Erreur : Impossible de charger le fichier son dice.mp3" << std::endl;
    }
    dice.setBuffer(diceBuffer);

    if (!gameboardSoundBuffer1.loadFromFile(soundPath + "/gameboard1.mp3")) {
        std::cerr << "Erreur : Impossible de charger le fichier son gameboard1.mp3" << std::endl;
    }
    gameboardSound1.setBuffer(gameboardSoundBuffer1);

    if (!gameboardSoundBuffer2.loadFromFile(soundPath + "/gameboard2.mp3")) {
        std::cerr << "Erreur : Impossible de charger le fichier son gameboard2.mp3" << std::endl;
    }
    gameboardSound2.setBuffer(gameboardSoundBuffer2);

    if (!gameboardSoundBuffer3.loadFromFile(soundPath + "/gameboard3.mp3")) {
        std::cerr << "Erreur : Impossible de charger le fichier son gameboard3.mp3" << std::endl;
    }
    gameboardSound3.setBuffer(gameboardSoundBuffer3);

    if (!gameboardSoundBuffer4.loadFromFile(soundPath + "/gameboard4.mp3")) {
        std::cerr << "Erreur : Impossible de charger le fichier son gameboard4.mp3" << std::endl;
    }
    gameboardSound4.setBuffer(gameboardSoundBuffer4);
}

void Menu::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::Resized) {
            handleResize(event.size.width, event.size.height);
        }

        if (currentState == MAIN_MENU) {
            handleMainMenuEvents(event, window);
        } else if (currentState == PLAYER_SELECTION) {
            handlePlayerSelectionEvents(event, window);
        }
    }
}

void Menu::handleMainMenuEvents(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (playAloneButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            clickSound.play();
            currentState = PLAYER_SELECTION;
            isPlayerComputer = true;
        } else if (playWithOthersButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            clickSound.play();
            currentState = PLAYER_SELECTION;
            isPlayerComputer = false;
        } else if (quitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            clickSound.play();
            while (clickSound.getStatus() == sf::Sound::Playing) {
                sf::sleep(sf::milliseconds(100));
            }
            window.close();
        }
    }
}

void Menu::handlePlayerSelectionEvents(sf::Event& event, sf::RenderWindow& window) {
    // Gestion des événements pour le menu de sélection des joueurs
    if (event.type == sf::Event::MouseButtonPressed) {
        // Exemple : revenir au menu principal
        currentState = MAIN_MENU;
    }
}

void Menu::handleResize(unsigned int newWidth, unsigned int newHeight) {
    sf::FloatRect visibleArea(0, 0, static_cast<float>(newWidth), static_cast<float>(newHeight));
    window.setView(sf::View(visibleArea));

    backgroundSprite.setScale(
        static_cast<float>(newWidth) / backgroundTexture.getSize().x,
        static_cast<float>(newHeight) / backgroundTexture.getSize().y
    );

    if (currentState == MAIN_MENU) {
        const sf::Vector2f buttonSize(200, 50);
        const float rightMargin = 70;
        const float buttonSpacing = 20;

        playAloneButton.setPosition(newWidth - buttonSize.x - rightMargin, 150);
        playAloneText.setPosition(playAloneButton.getPosition().x + 20, playAloneButton.getPosition().y + 10);
        neonTextPlayAlone.setPosition(playAloneButton.getPosition().x + 18, playAloneButton.getPosition().y + 8);

        playWithOthersButton.setPosition(playAloneButton.getPosition().x, playAloneButton.getPosition().y + buttonSize.y + buttonSpacing);
        playWithOthersText.setPosition(playWithOthersButton.getPosition().x + 20, playWithOthersButton.getPosition().y + 10);
        neonTextPlayWithOthers.setPosition(playWithOthersButton.getPosition().x + 18, playWithOthersButton.getPosition().y + 8);

        quitButton.setPosition(playWithOthersButton.getPosition().x, playWithOthersButton.getPosition().y + buttonSize.y + buttonSpacing);
        quitText.setPosition(quitButton.getPosition().x + 20, quitButton.getPosition().y + 10);
        neonTextQuit.setPosition(quitButton.getPosition().x + 18, quitButton.getPosition().y + 8);
    } else if (currentState == PLAYER_SELECTION) {
        // Implement resizing logic if needed
    }
}


void Menu::render(sf::RenderWindow& window) {
    window.clear(sf::Color::White);
    window.draw(backgroundSprite);

    if (currentState == MAIN_MENU) {
        renderMainMenu(window);
    } else if (currentState == PLAYER_SELECTION) {
        renderPlayerSelection(window);
    }

    window.display();
}

void Menu::renderMainMenu(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    neonTextPlayAlone.setFillColor(playAloneButton.getGlobalBounds().contains(mousePosF) ? sf::Color(0, 146, 50) : sf::Color::Transparent);
    neonTextPlayWithOthers.setFillColor(playWithOthersButton.getGlobalBounds().contains(mousePosF) ? sf::Color(0, 50, 174) : sf::Color::Transparent);
    neonTextQuit.setFillColor(quitButton.getGlobalBounds().contains(mousePosF) ? sf::Color(255, 0, 0) : sf::Color::Transparent);

    drawButtonMenu(window, playAloneButton, playAloneText, neonTextPlayAlone);
    drawButtonMenu(window, playWithOthersButton, playWithOthersText, neonTextPlayWithOthers);
    drawButtonMenu(window, quitButton, quitText, neonTextQuit);
}

void Menu::renderPlayerSelection(sf::RenderWindow& window) {
    for (size_t i = 0; i < playerButtons.size(); ++i) {
        window.draw(playerButtons[i]);
        window.draw(playerNames[i]);
    }
}

void Menu::drawButtonMenu(sf::RenderWindow& window, const sf::RectangleShape& button, sf::Text& text, sf::Text& neonText) {
    window.draw(neonText);
    window.draw(button);
    window.draw(text);
}