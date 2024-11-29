#include "jeu_petits_chevaux.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

JeuPetitsChevaux::JeuPetitsChevaux() : joueurActuel(0) {
    std::srand(std::time(nullptr));
}

void JeuPetitsChevaux::ajouterJoueur(std::shared_ptr<JoueurInterface> joueur) {
    joueurs.push_back(joueur);
}

void JeuPetitsChevaux::lancerDe() {
    if (joueurs.empty()) {
        std::cout << "Pas de joueurs dans le jeu.\n";
        return;
    }

    int valeurDe = std::rand() % 6 + 1;
    auto& joueur = joueurs[joueurActuel];
    std::cout << joueur->getNom() << " lance le dé et obtient un " << valeurDe << ".\n";
    joueur->avancer(valeurDe);
    std::cout << joueur->getNom() << " avance à la position " << joueur->getPosition() << ".\n";

    joueurActuel = (joueurActuel + 1) % joueurs.size();
}

void JeuPetitsChevaux::afficherPositions() const {
    for (const auto& joueur : joueurs) {
        std::cout << joueur->getNom() << " est à la position " << joueur->getPosition() << ".\n";
    }
}

bool JeuPetitsChevaux::estTermine() const {
    for (const auto& joueur : joueurs) {
        if (joueur->getPosition() == positionMax) {
            std::cout << joueur->getNom() << " a gagné!\n";
            return true;
        }
    }
    return false;
}
