#include "menu.hpp"

Menu::Menu()
    : window(sf::VideoMode(800, 600), "Petits Chevaux - Menu", sf::Style::Resize | sf::Style::Close) {
    window.setFramerateLimit(60);

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

    backgroundSprite.setTexture(backgroundTexture);

    // Initialisation des boutons et autres éléments
    const sf::Vector2f buttonSize(200, 50);
    const float rightMargin = 20;
    const float buttonSpacing = 20;

    // Initialisation du bouton "Play Alone"
    playAloneButton.setSize(buttonSize);
    playAloneButton.setFillColor(sf::Color::Blue);
    playAloneButton.setPosition(window.getSize().x - buttonSize.x - rightMargin, 150);

    playAloneText.setFont(font);
    playAloneText.setString("Play Alone");
    playAloneText.setCharacterSize(20);
    playAloneText.setFillColor(sf::Color::White);
    playAloneText.setPosition(playAloneButton.getPosition().x + 20, playAloneButton.getPosition().y + 10);

    // Initialisation du bouton "Play with Others"
    playWithOthersButton.setSize(buttonSize);
    playWithOthersButton.setFillColor(sf::Color::Green);
    playWithOthersButton.setPosition(playAloneButton.getPosition().x, playAloneButton.getPosition().y + buttonSize.y + buttonSpacing);

    playWithOthersText.setFont(font);
    playWithOthersText.setString("Play with Others");
    playWithOthersText.setCharacterSize(20);
    playWithOthersText.setFillColor(sf::Color::White);
    playWithOthersText.setPosition(playWithOthersButton.getPosition().x + 20, playWithOthersButton.getPosition().y + 10);

    // Initialisation du bouton "Quit"
    quitButton.setSize(buttonSize);
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setPosition(playWithOthersButton.getPosition().x, playWithOthersButton.getPosition().y + buttonSize.y + buttonSpacing);

    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(20);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(quitButton.getPosition().x + 20, quitButton.getPosition().y + 10);
}

void Menu::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

void Menu::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

            // Vérifier les clics sur les boutons
            if (playAloneButton.getGlobalBounds().contains(mousePosF)) {
                std::cout << "Play Alone cliqué !" << std::endl;
                startGame();  // Démarrer le jeu
            }

            if (playWithOthersButton.getGlobalBounds().contains(mousePosF)) {
                std::cout << "Play with Others cliqué !" << std::endl;
                startGame();  // Démarrer le jeu
            }

            if (quitButton.getGlobalBounds().contains(mousePosF)) {
                std::cout << "Quit cliqué !" << std::endl;
                window.close();
            }
        }

        // Gérer le redimensionnement de la fenêtre
        if (event.type == sf::Event::Resized) {
            // Adapter l'image de fond à la nouvelle taille de la fenêtre
            backgroundSprite.setScale(
                static_cast<float>(event.size.width) / backgroundTexture.getSize().x,
                static_cast<float>(event.size.height) / backgroundTexture.getSize().y
            );
        }
    }
}

void Menu::render() {
    window.clear(sf::Color::Black);

    // Afficher l'image de fond
    window.draw(backgroundSprite);

    // Afficher les boutons
    window.draw(playAloneButton);
    window.draw(playAloneText);
    window.draw(playWithOthersButton);
    window.draw(playWithOthersText);
    window.draw(quitButton);
    window.draw(quitText);

    window.display();
}

void Menu::startGame() {
    // Initialiser et démarrer le jeu
    Jeu jeu;
    jeu.run();
    window.close();  // Fermer le menu une fois le jeu lancé
}
