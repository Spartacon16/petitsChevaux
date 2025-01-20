#include "jeu.hpp"
#include "menu.hpp"
#include <vector>
// --- Pion ---
Pion::Pion(Couleur couleur, sf::Vector2f position)
    : couleur(couleur), position(position) {
    cercle.setRadius(15);
    cercle.setOrigin(15, 15);  // Centrer le cercle
    cercle.setPosition(position);

    switch (couleur) {
        case Couleur::Rouge: cercle.setFillColor(sf::Color::Red); break;
        case Couleur::Vert: cercle.setFillColor(sf::Color::Green); break;
        case Couleur::Bleu: cercle.setFillColor(sf::Color::Blue); break;
        case Couleur::Jaune: cercle.setFillColor(sf::Color::Yellow); break;
    }
}

void Pion::afficher(sf::RenderWindow& window) {
    window.draw(cercle);
}

void Pion::setPosition(sf::Vector2f newPosition) {
    position = newPosition;
    cercle.setPosition(newPosition);
}

// --- Plateau ---
Plateau::Plateau(int largeur, int hauteur) : largeur(largeur), hauteur(hauteur) {
    caseSize = 50; // Taille d'une case
    pions.resize(largeur * hauteur); // 1D pour simplifier
}

void Plateau::ajouterPion(int x, int y, Pion pion) {
    int index = y * largeur + x;
    pions[index].push_back(pion);
}

void Plateau::afficher(sf::RenderWindow& window) {
    // Dessiner les cases
    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            sf::RectangleShape caseRect(sf::Vector2f(caseSize, caseSize));
            caseRect.setPosition(x * caseSize, y * caseSize);
            caseRect.setFillColor(sf::Color(200, 200, 200));
            window.draw(caseRect);

            // Affichage des pions sur chaque case
            int index = y * largeur + x;
            for (auto& pion : pions[index]) {
                pion.afficher(window);
            }
        }
    }
}


Jeu::Jeu(sf::RenderWindow& window): window(window),plateau(800, 600){
    Plateau plateau(10, 10);

    // Initialisation des pions et des positions de départ
    pions.push_back(Pion(Pion::Couleur::Rouge, sf::Vector2f(25, 25)));
    pions.push_back(Pion(Pion::Couleur::Vert, sf::Vector2f(35, 25)));
    pions.push_back(Pion(Pion::Couleur::Bleu, sf::Vector2f(75, 75)));
    pions.push_back(Pion(Pion::Couleur::Jaune, sf::Vector2f(85, 75)));

    // Ajout des pions au plateau
    plateau.ajouterPion(0, 0, pions[0]);
    plateau.ajouterPion(0, 0, pions[1]);
    plateau.ajouterPion(1, 1, pions[2]);
    plateau.ajouterPion(1, 1, pions[3]);
}

void Jeu::run(sf::RenderWindow& window) {
    while (isRunning) {
        handleEvents(window);
        update();
        render(window);
    }
}

void Jeu::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            isRunning = false;
        }

        // Ajouter ici d'autres événements comme les clics ou les touches pour déplacer les pions
    }
}

void Jeu::update() {
    // Logique de mise à jour du jeu, par exemple pour gérer les déplacements des pions
    // Vous pouvez ajouter la gestion des déplacements, des tours, etc.
}

void Jeu::render(sf::RenderWindow& window) {
    window.clear(sf::Color::White);
    
    // Affichage du plateau
    plateau.afficher(window);

    // Affichage des pions
    for (auto& pion : pions) {
        pion.afficher(window);
    }

    window.display();
}
