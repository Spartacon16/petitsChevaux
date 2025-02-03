#include "menu.hpp"
#include <iostream>
#include <cmath>

Menu::Menu(sf::RenderWindow& window)
: window(window), currentState(MAIN_MENU), circlerayon(25), isPlayerComputer(true) 
  {
    // On charge la police
    if (!font.loadFromFile("../BrownieStencil-8O8MJ.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police.\n";
        exit(-1);
    }

    // Charger l'image de fond
    if (!backgroundTexture.loadFromFile("../menu_background.jpg")) {
        std::cerr << "Erreur : Impossible de charger le background.\n";
        exit(-1);
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(sf::Color(255, 255, 255, 200));
    backgroundSprite.setScale(
        float(window.getSize().x) / backgroundTexture.getSize().x,
        float(window.getSize().y) / backgroundTexture.getSize().y
    );

    // Initialisation des boutons du MAIN MENU
    const sf::Vector2f buttonSize(200, 50);
    const float rightMargin   = 70.f;
    const float buttonSpacing = 20.f;
    const int   fontSize      = 26;

    playAloneButton.setSize(buttonSize);
    playAloneButton.setFillColor(sf::Color::Transparent);
    playAloneButton.setPosition(window.getSize().x - buttonSize.x - rightMargin, 150);

    playAloneText.setFont(font);
    playAloneText.setString("Play Alone");
    playAloneText.setCharacterSize(fontSize);
    playAloneText.setStyle(sf::Text::Bold);
    playAloneText.setFillColor(sf::Color::White);
    playAloneText.setPosition(
        playAloneButton.getPosition().x + 20,
        playAloneButton.getPosition().y + 10
    );
    neonTextPlayAlone = playAloneText;
    neonTextPlayAlone.setPosition(
        playAloneText.getPosition().x - 2,
        playAloneText.getPosition().y - 2
    );

    playWithOthersButton = playAloneButton;
    playWithOthersButton.setPosition(
        playAloneButton.getPosition().x,
        playAloneButton.getPosition().y + buttonSize.y + buttonSpacing
    );
    playWithOthersText = playAloneText;
    playWithOthersText.setString("Play with Others");
    playWithOthersText.setPosition(
        playWithOthersButton.getPosition().x + 20,
        playWithOthersButton.getPosition().y + 10
    );
    neonTextPlayWithOthers = playWithOthersText;
    neonTextPlayWithOthers.setPosition(
        playWithOthersText.getPosition().x - 2,
        playWithOthersText.getPosition().y - 2
    );

    quitButton = playAloneButton;
    quitButton.setPosition(
        playWithOthersButton.getPosition().x,
        playWithOthersButton.getPosition().y + buttonSize.y + buttonSpacing
    );
    quitText = playAloneText;
    quitText.setString("Quit");
    quitText.setPosition(
        quitButton.getPosition().x + 20,
        quitButton.getPosition().y + 10
    );
    neonTextQuit = quitText;
    neonTextQuit.setPosition(
        quitText.getPosition().x - 2,
        quitText.getPosition().y - 2
    );

    // Boutons de la Player Selection
    Xbutton.setRadius(circlerayon);
    Xbutton.setOrigin(circlerayon, circlerayon); // centre au milieu
    Xbutton.setFillColor(sf::Color::Red);

    // Bouton + (ajouter joueur)
    plusButton.setSize(sf::Vector2f(50.f, 50.f));
    plusButton.setFillColor(sf::Color::Transparent);
    plusButton.setPosition(100.f, 200.f);
    plusText.setFont(font);
    plusText.setString("+");
    plusText.setCharacterSize(50);
    plusText.setFillColor(sf::Color::White);
    plusText.setPosition(
        plusButton.getPosition().x + 17,
        plusButton.getPosition().y + 5
    );
    neonPlusText = plusText;
    neonPlusText.setPosition(
        plusText.getPosition().x - 2,
        plusText.getPosition().y - 2
    );

    // Bouton - (retirer joueur)
    minusButton = plusButton;
    minusButton.setPosition(plusButton.getPosition().x + 80.f, plusButton.getPosition().y);
    minusText = plusText;
    minusText.setString("-");
    minusText.setPosition(
        minusButton.getPosition().x + 17,
        minusButton.getPosition().y + 5
    );
    neonMinusText = minusText;
    neonMinusText.setPosition(
        minusText.getPosition().x - 2,
        minusText.getPosition().y - 2
    );

    // Bouton "START"
    startButton.setSize(sf::Vector2f(150.f, 50.f));
    startButton.setFillColor(sf::Color::Transparent);
    startButton.setPosition(100.f, 300.f);

    startText.setFont(font);
    startText.setString("Start");
    startText.setCharacterSize(fontSize);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(
        startButton.getPosition().x + 25,
        startButton.getPosition().y + 8
    );
    neonTextStart = startText;
    neonTextStart.setPosition(
        startText.getPosition().x - 2,
        startText.getPosition().y - 2
    );

    // Charger les sons
    loadSound("../bruits");
    // Charger la texture du pion
    loadTextures();
    initPlayers();
}

void Menu::loadSound(const std::string& soundPath) {
    if (!clickBuffer.loadFromFile(soundPath + "/clic.mp3")) {
        std::cerr << "Impossible de charger clic.mp3\n";
    }
    clickSound.setBuffer(clickBuffer);

    if (!criBuffer.loadFromFile(soundPath + "/cri.mp3")) {
        std::cerr << "Impossible de charger cri.mp3\n";
    }
    cri.setBuffer(criBuffer);

    if (!diceBuffer.loadFromFile(soundPath + "/dice.mp3")) {
        std::cerr << "Impossible de charger dice.mp3\n";
    }
    dice.setBuffer(diceBuffer);

    std::vector<std::string> soundFiles = {
        "gameboard1.mp3", "gameboard2.mp3", "gameboard3.mp3", "gameboard4.mp3"
    };

    gameboardSoundBuffers.clear();
    gameboardSounds.clear();

    for (const auto& file : soundFiles) {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(soundPath + "/" + file)) {
            std::cerr << "Impossible de charger " << file << "\n";
            continue;
        }
        gameboardSoundBuffers.push_back(buffer);
        sf::Sound sound;
        sound.setBuffer(gameboardSoundBuffers.back());
        gameboardSounds.push_back(sound);
    }
}


void Menu::loadTextures()
{
    // Charger la texture du pion (unique) ; on appliquera une couleur sur le sprite.
    if (!pionTexture.loadFromFile("../pion.png")) {
        std::cerr << "Erreur: Impossible de charger pion.png\n";
        // On ne fait pas exit(-1) pour laisser le menu tourner malgré tout
    }
}

void Menu::handleEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::Resized) {
            handleResize(event.size.width, event.size.height);
        }

        // Selon l’état du menu
        if (currentState == MAIN_MENU) {
            handleMainMenuEvents(event, window);
        }
        else if (currentState == PLAYER_SELECTION) {
            handlePlayerSelectionEvents(event, window);
        }
    }
}

void Menu::handleMainMenuEvents(sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (playAloneButton.getGlobalBounds().contains(mousePos))
        {
            clickSound.play();
            // On va en Player Selection mode “ordinateurs”
            isPlayerComputer = true;
            initPlayers(); // Crée 2 joueurs par défaut (J1, COM1)
            currentState = PLAYER_SELECTION;
        }
        else if (playWithOthersButton.getGlobalBounds().contains(mousePos))
        {
            clickSound.play();
            // On va en Player Selection mode “multi local”
            isPlayerComputer = false;
            initPlayers(); // Crée 2 joueurs par défaut (J1, J2)
            currentState = PLAYER_SELECTION;
        }
        else if (quitButton.getGlobalBounds().contains(mousePos))
        {
            clickSound.play();
            while (clickSound.getStatus() == sf::Sound::Playing) {
                sf::sleep(sf::milliseconds(50));
            }
            window.close();
        }
    }
}

void Menu::handlePlayerSelectionEvents(sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Bouton X (retour au menu)
        sf::Vector2f center = Xbutton.getPosition(); // centre du bouton
        float dx = mousePos.x - center.x;
        float dy = mousePos.y - center.y;
        if (dx*dx + dy*dy <= float(circlerayon*circlerayon))
        {
            currentState = MAIN_MENU;
            return;
        }

        // Bouton +
        if (plusButton.getGlobalBounds().contains(mousePos)) {
            clickSound.play();
            addPlayer();
            return;
        }
        // Bouton -
        if (minusButton.getGlobalBounds().contains(mousePos)) {
            clickSound.play();
            removePlayer();
            return;
        }

        // Bouton Start
        if (startButton.getGlobalBounds().contains(mousePos)) {
            clickSound.play();
            // On lance réellement le jeu => main.cpp fera jeu.run(players)
            currentState = GAME_RUNNING;
            return;
        }
    }
}

void Menu::initPlayers()
{
    players.clear();
     // Couleurs pastel dans l'ordre demandé
    std::vector<sf::Color> pastelColors = {
        sf::Color(255, 255, 153),   // Jaune pastel
        sf::Color(255, 153, 153),   // Rouge pastel
        sf::Color(153, 255, 153),   // Vert pastel
        sf::Color(153, 204, 255)    // Bleu pastel
    };

    if (isPlayerComputer) {
        // “Play Alone” => J1, COM1 (2 joueurs par défaut)
        PlayerInfo p1;
        p1.name   = "J1";
        p1.color  = pastelColors[0];
        p1.sprite.setTexture(pionTexture);
        p1.sprite.setColor(p1.color);
        p1.sprite.setOrigin(16, 16);
        p1.sprite.setScale(0.08f,0.08f);
        players.push_back(p1);

        PlayerInfo p2;
        p2.name   = "COM1";
        p2.color  = pastelColors[1];
        p2.sprite.setTexture(pionTexture);
        p2.sprite.setColor(p2.color);
        p2.sprite.setOrigin(16, 16);
        p2.sprite.setScale(0.08f,0.08f);
        
        players.push_back(p2);
    } else {
        // “Play with Others” => J1, J2
        PlayerInfo p1;
        p1.name   = "J1";
        p1.color  = pastelColors[0];
        p1.sprite.setTexture(pionTexture);
        p1.sprite.setColor(p1.color);
        p1.sprite.setOrigin(16, 16);
        p1.sprite.setScale(0.08f,0.08f);
        players.push_back(p1);

        PlayerInfo p2;
        p2.name   = "J2";
        p2.color  = pastelColors[1];
        p2.sprite.setTexture(pionTexture);
        p2.sprite.setColor(p2.color);
        p2.sprite.setOrigin(16, 16);
        p2.sprite.setScale(0.08f,0.08f);
        players.push_back(p2);
    }
    updatePlayerSpritesPositions();
}

void Menu::addPlayer()
{
    if (players.size() >= 4) {
        std::cout << "Déjà 4 joueurs, impossible d'en ajouter.\n";
        return;
    }

    std::vector<sf::Color> pastelColors = {
        sf::Color(255, 255, 153),   // Jaune pastel
        sf::Color(255, 153, 153),   // Rouge pastel
        sf::Color(153, 255, 153),   // Vert pastel
        sf::Color(153, 204, 255)    // Bleu pastel
    };
    // Nouveau joueur => On détermine le next index
    int index = (int)players.size(); // 2 => 2e ajout ; 3 => 3e ajout ...
    
    

    PlayerInfo newP;
    newP.sprite.setTexture(pionTexture);
    newP.sprite.setOrigin(16,16);
    newP.sprite.setScale(0.08f,0.08f);

    if (isPlayerComputer) {
        // J1 déjà créé => ensuite COM1, COM2, COM3…
        // On a déjà 2 joueurs => index=2 => COM2
        // index=3 => COM3
        newP.color = pastelColors[index];   // ex: Blue => 4e
        newP.sprite.setColor(newP.color);

        newP.name = "COM" + std::to_string(index);
    }
    else {
        // J1, J2 => puis J3, J4
        newP.color = pastelColors[index];
        newP.sprite.setColor(newP.color);

        newP.name = "J" + std::to_string(index+1); 
        // si index=2 => “J3”, etc.
    }

    players.push_back(newP);
    updatePlayerSpritesPositions();
}

void Menu::removePlayer()
{
    // On ne retire pas si on est déjà à 1 joueur
    if (players.size() <= 2) {
        std::cout << "Impossible de tomber en-dessous de 2 joueur mate.\n";
        return;
    }
    players.pop_back();
    updatePlayerSpritesPositions();
}

void Menu::updatePlayerSpritesPositions()
{
    // Juste un pti algo pour positionner les pions en colonne
    float startX = 500.f;
    float startY = 120.f;
    float offsetY = 120.f;

    for (size_t i = 0; i < players.size(); i++) {
        players[i].sprite.setPosition(startX, startY + i * offsetY);
    }
}

void Menu::handleResize(unsigned int newWidth, unsigned int newHeight)
{
    sf::FloatRect visibleArea(0, 0, float(newWidth), float(newHeight));
    window.setView(sf::View(visibleArea));

    backgroundSprite.setScale(
        float(newWidth)  / backgroundTexture.getSize().x,
        float(newHeight) / backgroundTexture.getSize().y
    );
}

void Menu::render(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    window.draw(backgroundSprite);

    if (currentState == MAIN_MENU) {
        renderMainMenu(window);
    }
    else if (currentState == PLAYER_SELECTION) {
        renderPlayerSelection(window);
    }

    window.display();
}

void Menu::renderMainMenu(sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    neonTextPlayAlone.setFillColor(
        playAloneButton.getGlobalBounds().contains(mousePos)
            ? sf::Color(0, 255, 50)
            : sf::Color::Transparent
    );
    neonTextPlayWithOthers.setFillColor(
        playWithOthersButton.getGlobalBounds().contains(mousePos)
            ? sf::Color(0, 50, 255)
            : sf::Color::Transparent
    );
    neonTextQuit.setFillColor(
        quitButton.getGlobalBounds().contains(mousePos)
            ? sf::Color(255, 0, 0)
            : sf::Color::Transparent
    );

    drawButtonMenu(window, playAloneButton, playAloneText, neonTextPlayAlone);
    drawButtonMenu(window, playWithOthersButton, playWithOthersText, neonTextPlayWithOthers);
    drawButtonMenu(window, quitButton, quitText, neonTextQuit);
}

void Menu::renderPlayerSelection(sf::RenderWindow& window)
{
    // Bouton X (fermeture => retour menu)
    Xbutton.setPosition(window.getSize().x - 40, 40);
    window.draw(Xbutton);

    // Dessin d'une croix blanche
    sf::RectangleShape line1(sf::Vector2f(35.f, 5.f));
    sf::RectangleShape line2(sf::Vector2f(35.f, 5.f));
    line1.setFillColor(sf::Color::White);
    line2.setFillColor(sf::Color::White);
    line1.setOrigin(17.5f, 2.5f);
    line2.setOrigin(17.5f, 2.5f);
    line1.setPosition(Xbutton.getPosition());
    line2.setPosition(Xbutton.getPosition());
    line1.setRotation(45.f);
    line2.setRotation(-45.f);
    window.draw(line1);
    window.draw(line2);

    // Boutons + et -
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    neonPlusText.setFillColor(
        plusButton.getGlobalBounds().contains(mousePos)
            ? sf::Color::Yellow
            : sf::Color::Transparent
    );
    neonMinusText.setFillColor(
        minusButton.getGlobalBounds().contains(mousePos)
            ? sf::Color::Yellow
            : sf::Color::Transparent
    );
    window.draw(plusButton);
    window.draw(minusButton);
    window.draw(neonPlusText);
    window.draw(neonMinusText);
    window.draw(plusText);
    window.draw(minusText);

    // Bouton "START"
    neonTextStart.setFillColor(
        startButton.getGlobalBounds().contains(mousePos)
            ? sf::Color::Cyan
            : sf::Color::Transparent
    );
    drawButtonMenu(window, startButton, startText, neonTextStart);

    // Affichage des joueurs (pions + nom)
    for (auto &p : players)
    {
        window.draw(p.sprite);

        // On dessine le nom du joueur à côté de son pion
        sf::Text txt;
        txt.setFont(font);
        txt.setString(p.name);
        txt.setCharacterSize(50);
        txt.setFillColor(sf::Color::White);

        sf::Vector2f pos = p.sprite.getPosition();
        txt.setPosition(pos.x + 100.f, pos.y+10.f);

        window.draw(txt);
    }
}

void Menu::drawButtonMenu(sf::RenderWindow& window,
                          const sf::RectangleShape& button,
                          sf::Text& text,
                          sf::Text& neonText)
{
    // “Halo” coloré
    window.draw(neonText);
    // Fond du bouton
    window.draw(button);
    // Texte du bouton
    window.draw(text);
}
