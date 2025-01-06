#include "menu.hpp"

Menu::Menu(sf::RenderWindow& window) {
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
    backgroundSprite.setTexture(backgroundTexture); // Initialisation des boutons et autres éléments

    const sf::Vector2f buttonSize(200, 50);
    const float rightMargin = 70;
    const float buttonSpacing = 20;
    const int fontsize=26;

    playAloneButton.setSize(buttonSize);
    playAloneButton.setFillColor(sf::Color::Transparent);  
    playAloneButton.setPosition(window.getSize().x - buttonSize.x - rightMargin, 150);
    playAloneText.setFont(font);
    playAloneText.setString("Play Alone");
    playAloneText.setCharacterSize(fontsize);
    playAloneText.setStyle(sf::Text::Bold);
    playAloneText.setFillColor(sf::Color::White);
    playAloneText.setPosition(playAloneButton.getPosition().x + 20, playAloneButton.getPosition().y + 10);
    neonTextPlayAlone=playAloneText;
    neonTextPlayAlone.setPosition(playAloneButton.getPosition().x + 18, playAloneButton.getPosition().y + 8);

    playWithOthersButton=playAloneButton;
    playWithOthersButton.setPosition(playAloneButton.getPosition().x, playAloneButton.getPosition().y + buttonSize.y + buttonSpacing);
    playWithOthersText=playAloneText;
    playWithOthersText.setString("Play with Others");
    playWithOthersText.setPosition(playWithOthersButton.getPosition().x + 20, playWithOthersButton.getPosition().y + 10);
    neonTextPlayWithOthers=playWithOthersText;
    neonTextPlayWithOthers.setPosition(playWithOthersButton.getPosition().x + 18, playWithOthersButton.getPosition().y + 8);

    quitButton=playAloneButton;
    quitButton.setPosition(playWithOthersButton.getPosition().x, playWithOthersButton.getPosition().y + buttonSize.y + buttonSpacing);
    quitText=playAloneText;
    quitText.setString("Quit");
    quitText.setPosition(quitButton.getPosition().x + 20, quitButton.getPosition().y + 10);
    neonTextQuit=quitText;
    neonTextQuit.setPosition(quitButton.getPosition().x + 18, quitButton.getPosition().y + 8);

    // Initialisation du menu de sélection des joueurs
    isPlayerSelectionActive = false;
}

void Menu::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Gérer les clics sur les boutons
            if (playAloneButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                std::cout << "Play Alone cliqué !" << std::endl;
                startGame(); // Passer à la sélection de joueurs
            }
            else if (playWithOthersButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                switchToPlayerSelection(); // Passer au menu de sélection des joueurs
            }
            else if (quitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                window.close(); // Quitter le jeu
            }
        }// Gérer le redimensionnement de la fenêtre
        if (event.type == sf::Event::Resized) {
            // Adapter l'image de fond à la nouvelle taille de la fenêtre
            backgroundSprite.setScale(
                static_cast<float>(event.size.width) / backgroundTexture.getSize().x,
                static_cast<float>(event.size.height) / backgroundTexture.getSize().y
            );
        }
    }
}

void Menu::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    // Afficher le fond et les boutons
        // Afficher l'image de fond
    window.draw(backgroundSprite);

    // Détecter si la souris survole les boutons pour appliquer l'effet néon
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    // Effet néon pour chaque bouton
    if (playAloneButton.getGlobalBounds().contains(mousePosF)) {
        neonTextPlayAlone.setFillColor(sf::Color(0,  146,50));  // Cyan néon
    } else {
        neonTextPlayAlone.setFillColor(sf::Color::Transparent);  // Aucun néon si pas de survol
    }

    if (playWithOthersButton.getGlobalBounds().contains(mousePosF)) {
        neonTextPlayWithOthers.setFillColor(sf::Color(0,50,174));  // Cyan néon
    } else {
        neonTextPlayWithOthers.setFillColor(sf::Color::Transparent);
    }

    if (quitButton.getGlobalBounds().contains(mousePosF)) {
        neonTextQuit.setFillColor(sf::Color(255, 0,0));  // Rouge néon
    } else {
        neonTextQuit.setFillColor(sf::Color::Transparent);
    }

    // Dessiner les boutons
    drawButton(window,playAloneButton, playAloneText, neonTextPlayAlone);
    drawButton(window,playWithOthersButton, playWithOthersText, neonTextPlayWithOthers);
    drawButton(window,quitButton, quitText, neonTextQuit);

    window.display();
}

void Menu::drawButton(sf::RenderWindow& window,const sf::RectangleShape& button, sf::Text& text, sf::Text& neonText) {
    window.draw(neonText);  // Afficher le texte néon
    window.draw(button);    // Afficher le bouton
    window.draw(text);      // Afficher le texte normal
}

void Menu::update() {
    // Mettre à jour des éléments comme des animations ou des changements
}

void Menu::switchToPlayerSelection() {
    isPlayerSelectionActive = true;  // Activer le menu de sélection des joueurs
}

void Menu::startGame() {
    // Démarrer le jeu (ce code sera à développer en fonction des besoins du jeu)
}


// PlayerSelectionMenu::PlayerSelectionMenu() {
     

//     // Initialisation des couleurs des joueurs
//     playerColors = {sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow};

//     playerCount = 1;  // Nombre de joueurs initialisé à 1

//     // Initialisation des boutons pour la sélection des joueurs
//     for (int i = 0; i < playerCount; ++i) {
//         sf::RectangleShape button(sf::Vector2f(100, 100));
//         button.setFillColor(playerColors[i]);
//         button.setPosition(150 + i * 120, 100);

//         playerButtons.push_back(button);

//         sf::Text text;
//         text.setFont(font);
//         text.setString("J" + std::to_string(i + 1));
//         text.setCharacterSize(24);
//         text.setFillColor(sf::Color::Black);
//         text.setPosition(150 + i * 120, 160);
//         playerText.push_back(text);
//     }

//     // Initialisation des boutons + et -
//     addButton.setSize(sf::Vector2f(50, 50));
//     addButton.setFillColor(sf::Color::Green);
//     addButton.setPosition(150, 250);

//     addButtonText.setFont(font);
//     addButtonText.setString("+");
//     addButtonText.setCharacterSize(30);
//     addButtonText.setFillColor(sf::Color::White);
//     addButtonText.setPosition(160, 260);

//     removeButton.setSize(sf::Vector2f(50, 50));
//     removeButton.setFillColor(sf::Color::Red);
//     removeButton.setPosition(220, 250);

//     removeButtonText.setFont(font);
//     removeButtonText.setString("-");
//     removeButtonText.setCharacterSize(30);
//     removeButtonText.setFillColor(sf::Color::White);
//     removeButtonText.setPosition(230, 260);
// }

// void PlayerSelectionMenu::handleEvents(sf::RenderWindow& window) {
//     sf::Event event;
//     while (window.pollEvent(event)) {
//         if (event.type == sf::Event::Closed)
//             window.close();
//         if (event.type == sf::Event::MouseButtonPressed) {
//             sf::Vector2i mousePos = sf::Mouse::getPosition(window);

//             // Gérer les clics sur les boutons de sélection de couleur
//             for (size_t i = 0; i < playerButtons.size(); ++i) {
//                 if (playerButtons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
//                     handleColorSelection(i);  // Sélectionner une couleur pour ce joueur
//                 }
//             }

//             // Ajouter un joueur
//             if (addButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
//                 if (playerCount < 4) {
//                     playerCount++;
//                     playerButtons.push_back(sf::RectangleShape(sf::Vector2f(100, 100)));
//                     playerText.push_back(sf::Text());
//                 }
//             }

//             // Supprimer un joueur
//             if (removeButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
//                 if (playerCount > 1) {
//                     playerCount--;
//                     playerButtons.pop_back();
//                     playerText.pop_back();
//                 }
//             }
//         }
//         // Gérer le redimensionnement de la fenêtre
//         if (event.type == sf::Event::Resized) {
//             // Adapter l'image de fond à la nouvelle taille de la fenêtre
//             backgroundSprite.setScale(
//                 static_cast<float>(event.size.width) / backgroundTexture.getSize().x,
//                 static_cast<float>(event.size.height) / backgroundTexture.getSize().y
//             );
//         }
//     }
// }

// void PlayerSelectionMenu::render(sf::RenderWindow& window) {
//     window.clear();

//     // Afficher les boutons des joueurs et les boutons + et -
//     for (size_t i = 0; i < playerButtons.size(); ++i) {
//         window.draw(playerButtons[i]);
//         window.draw(playerText[i]);
//     }

//     window.draw(addButton);
//     window.draw(addButtonText);
//     window.draw(removeButton);
//     window.draw(removeButtonText);

//     window.display();
// }

// void PlayerSelectionMenu::update() {
//     // Mettre à jour des éléments si nécessaire (animation, etc.)
// }

// void PlayerSelectionMenu::handleColorSelection(int playerIndex) {
//     // Exemple de code pour gérer la sélection de couleur pour chaque joueur
//     if (playerColors[playerIndex] == sf::Color::Red) {
//         playerButtons[playerIndex].setFillColor(sf::Color::Green);  // Changer la couleur, à ajuster selon la logique
//     } else {
//         playerButtons[playerIndex].setFillColor(sf::Color::Red);
//     }
// }
