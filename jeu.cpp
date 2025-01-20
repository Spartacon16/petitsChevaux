#include "jeu.hpp"
#include <iostream>

// Constructeur
Jeu::Jeu(sf::RenderWindow& window)
: window(window), isRunning(false) {
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
        { 100.f, 100.f }, // Haut gauche : Jaune (J1)
        { 450.f, 450.f }, // Bas droit : Rouge (J2)
        { 100.f, 450.f }, // Bas gauche : Vert (J3)
        { 450.f, 100.f }  // Haut droit : Bleu (J4)
    };

    // Couleurs pastel des joueurs
    playerColors = {
        sf::Color(255, 255, 153),   // Jaune pastel (J1)
        sf::Color(255, 102, 102),   // Rouge pastel (J2)
        sf::Color(102, 255, 102),   // Vert pastel (J3)
        sf::Color(102, 178, 255)    // Bleu pastel (J4)
    };
}

// Configurer les joueurs et leurs pions
void Jeu::setPlayers(const std::vector<PlayerInfo>& playersSelected) {
    playersInGame = playersSelected;
    playerPions.clear(); // Réinitialiser les pions pour chaque joueur

    for (size_t i = 0; i < playersInGame.size(); ++i) {
        std::vector<PionInfo> pionsJoueur; // Les 4 pions du joueur

        for (int j = 0; j < 4; ++j) { // Chaque joueur a 4 pions
            PionInfo pion;
            pion.sprite.setTexture(pionTexture);

            // Centrer le sprite : le pion fait 800×800 => origine = (400,400)
            pion.sprite.setOrigin(400.f, 400.f);

            // Appliquer l'échelle pour ajuster la taille
            pion.sprite.setScale(pionScale, pionScale);

            // Appliquer la couleur pastel du joueur
            pion.sprite.setColor(playerColors[i]);

            // Positionner chaque pion dans une zone de départ
            pion.startPosition = {
                startingPositions[i].x + (j % 2) * 50.f, // Décalage horizontal
                startingPositions[i].y + (j / 2) * 50.f  // Décalage vertical
            };
            pion.sprite.setPosition(pion.startPosition);

            pionsJoueur.push_back(pion);
        }

        playerPions.push_back(pionsJoueur); // Ajouter les pions du joueur
    }
}

// Obtenir l'échelle des pions
float Jeu::getPionScale() const {
    return pionScale;
}

// Obtenir les couleurs des joueurs
const std::vector<sf::Color>& Jeu::getPlayerColors() const {
    return playerColors;
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

// Gestion des événements
void Jeu::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

// Mettre à jour la logique du jeu (placeholder pour futur ajout)
void Jeu::update() {
    // Ici, ajoutez les règles du jeu ou la logique de déplacement
}

// Rendu graphique
void Jeu::render(sf::RenderWindow& window) {
    window.clear();

    // Dessiner le plateau
    window.draw(plateauSprite);

    // Dessiner les pions
    for (const auto& pionsJoueur : playerPions) {
        for (const auto& pion : pionsJoueur) {
            window.draw(pion.sprite);
        }
    }

    window.display();
}

// Obtenir la position d'une case (placeholder pour futur ajout)
sf::Vector2f Jeu::getCasePosition(int caseIndex, int playerStartIndex) {
    // À implémenter pour déplacer les pions sur le plateau
    return sf::Vector2f(0, 0);
}
