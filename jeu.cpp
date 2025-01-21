#include "jeu.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Constructeur
Jeu::Jeu(sf::RenderWindow& window, const sf::Font& font)
    : window(window), font(font), isRunning(false), joueurActuel(0), valeurDe(0) {
    // Initialisation de la graine pour les nombres aleatoires
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Charger la texture du plateau
    if (!plateauTexture.loadFromFile("../table_jeu.png")) {
        std::cerr << "Erreur : Impossible de charger table_jeu.png\n";
    }
    plateauSprite.setTexture(plateauTexture);

    // Redimensionner le plateau à 600x600
    sf::Vector2u texSize = plateauTexture.getSize(); // Taille originale (par ex. 225×225)
    float scaleX = 600.f / texSize.x;
    float scaleY = 600.f / texSize.y;
    plateauSprite.setScale(scaleX, scaleY);

    // Charger la texture des pions
    if (!pionTexture.loadFromFile("../Pion.png")) {
        std::cerr << "Erreur : Impossible de charger Pion.png\n";
    }

    // Taille d'un carreau
    const float carreauPx = 50.f;

    // Calcul de l'échelle pour ajuster le pion à un carreau
    pionScale = carreauPx / 800.f; // 800 px correspond à la taille de l'image Pion.png

    // Positions de départ pour chaque joueur (zones de départ, en carré)
    startingPositions = {
        {100.f, 100.f}, // Jaune
        {450.f, 450.f}, // Rouge
        {100.f, 450.f}, // Vert
        {450.f, 100.f}  // Bleu
    };

    // Couleurs des joueurs
    playerColors = {
        sf::Color(255, 255, 153),   // Jaune pastel
        sf::Color(255, 102, 102),   // Rouge pastel
        sf::Color(102, 255, 102),   // Vert pastel
        sf::Color(102, 178, 255)    // Bleu pastel
    };

    // Bouton "Lancer le de"
    boutonLancerDe.setSize(sf::Vector2f(150.f, 50.f));
    boutonLancerDe.setPosition(610.f, 300.f);
    boutonLancerDe.setFillColor(sf::Color::White);
    boutonLancerDe.setOutlineThickness(2);
    boutonLancerDe.setOutlineColor(sf::Color::Black);

    // Texte du bouton
    texteLancerDe.setFont(font);
    texteLancerDe.setString("Lancer le de");
    texteLancerDe.setCharacterSize(20);
    texteLancerDe.setFillColor(sf::Color::Black);
    texteLancerDe.setPosition(boutonLancerDe.getPosition().x + 10, boutonLancerDe.getPosition().y + 10);

    // Texte des actions
    texteActions.setFont(font);
    texteActions.setCharacterSize(20);
    texteActions.setFillColor(sf::Color::Black);
    texteActions.setPosition(610.f, 50.f);
    texteActions.setString("Joueur: J1\n=> Lancez le de");

    setupPlateau();
}

// Configurer les joueurs et leurs pions
void Jeu::setPlayers(const std::vector<PlayerInfo>& playersSelected) {
    playersInGame = playersSelected;
    playerPions.clear(); // Reinitialiser les pions pour tous les joueurs

    for (size_t i = 0; i < playersInGame.size(); ++i) {
        std::vector<PionInfo> pionsJoueur; // 4 pions par joueur

        for (int j = 0; j < 4; ++j) {
            PionInfo pion;
            pion.sprite.setTexture(pionTexture);
            pion.sprite.setOrigin(400.f, 400.f); // Centrer le sprite
            pion.sprite.setScale(pionScale, pionScale); // Ajuster la taille
            pion.sprite.setColor(playerColors[i]);

            // Position des pions dans la prison
            pion.startPosition = {
                startingPositions[i].x + (j % 2) * 50.f,
                startingPositions[i].y + (j / 2) * 50.f
            };
            pion.sprite.setPosition(pion.startPosition);

            pionsJoueur.push_back(pion);
        }

        playerPions.push_back(pionsJoueur);
    }
}

// Gérer les événements
void Jeu::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (boutonLancerDe.getGlobalBounds().contains(mousePos)) {
                lancerDe();
            }
        }
    }
}

// Lancer le dé
void Jeu::lancerDe() {
    // Generer un nombre aleatoire entre 1 et 6
    valeurDe = rand() % 6 + 1;

    // Mettre a jour le texte d'action
    std::string message = "Joueur: " + playersInGame[joueurActuel].name + "\nDe: " + std::to_string(valeurDe);

    if (valeurDe == 6) {
        message += "\n=> Sortez un pion de la prison !";
        // Ajouter ici la logique pour permettre au joueur de sortir un pion
    } else {
        message += "\n=> Tour suivant.";
        passerAuJoueurSuivant();
    }

    texteActions.setString(message);
}

// Passer au joueur suivant
void Jeu::passerAuJoueurSuivant() {
    joueurActuel = (joueurActuel + 1) % playersInGame.size();
    texteActions.setString("Joueur: " + playersInGame[joueurActuel].name + "\n=> Lancez le de");
}

// Afficher le jeu
void Jeu::render(sf::RenderWindow& window) {
    window.clear(sf::Color::White); // Fond blanc

    // Dessiner le plateau
    window.draw(plateauSprite);

    // Dessiner les pions
    for (const auto& pionsJoueur : playerPions) {
        for (const auto& pion : pionsJoueur) {
            window.draw(pion.sprite);
        }
    }

    // Dessiner le bouton et le texte
    window.draw(boutonLancerDe);
    window.draw(texteLancerDe);
    window.draw(texteActions);

    window.display();
}

// Configurer le plateau
void Jeu::setupPlateau() {
    generateParcours();

    // Ajouter la case "Victoire" au centre
    cases.emplace_back(sf::Vector2f(300.f, 300.f), "Victoire");
}

// Generer les cases du parcours
void Jeu::generateParcours() {
    const float delta = 14.f;
    sf::Vector2f startingPositions[4] = {
        {16.5f, 98.5f},  // Jaune
        {98.5f, 16.5f},  // Bleu
        {500.5f, 98.5f}, // Rouge
        {98.5f, 500.5f}  // Vert
    };

    for (int player = 0; player < 4; ++player) {
        sf::Vector2f currentPos = startingPositions[player];

        // Cases 1 a 7
        for (int i = 1; i <= 7; ++i) {
            cases.emplace_back(currentPos, "Parcours", player);
            switch (player) {
                case 0: currentPos.x += delta; break; // Jaune
                case 1: currentPos.y -= delta; break; // Bleu
                case 2: currentPos.x -= delta; break; // Rouge
                case 3: currentPos.y += delta; break; // Vert
            }
        }

        // Cases 8 a 13
        for (int i = 8; i <= 13; ++i) {
            cases.emplace_back(currentPos, "Parcours", player);
            switch (player) {
                case 0: currentPos.y += delta; break;
                case 1: currentPos.x += delta; break;
                case 2: currentPos.y -= delta; break;
                case 3: currentPos.x -= delta; break;
            }
        }

        // Case 14
        switch (player) {
            case 0: currentPos.y -= delta; break;
            case 1: currentPos.x -= delta; break;
            case 2: currentPos.y += delta; break;
            case 3: currentPos.x += delta; break;
        }
        cases.emplace_back(currentPos, "Parcours", player);
    }
}

// Lancer la boucle du jeu
void Jeu::run(sf::RenderWindow& window) {
    isRunning = true;

    while (window.isOpen() && isRunning) {
        handleEvents(window);
        update();
        render(window);
    }
}

// Mettre à jour la logique du jeu (placeholder pour futur ajout)
void Jeu::update() {
    // Ici, ajoutez les règles du jeu ou la logique de déplacement
}

