#include "jeu.hpp"

Jeu::Jeu()
    : window(sf::VideoMode(800, 600), "Petits Chevaux"), plateau(10, 10), isRunning(true) {

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

void Jeu::run() {
    while (isRunning) {
        handleEvents();
        update();
        render();
    }
}

void Jeu::handleEvents() {
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

void Jeu::render() {
    window.clear(sf::Color::White);
    
    // Affichage du plateau
    plateau.afficher(window);

    // Affichage des pions
    for (auto& pion : pions) {
        pion.afficher(window);
    }

    window.display();
}
