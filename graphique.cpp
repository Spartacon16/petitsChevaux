#include "graphique.hpp"

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
