#include "joueur_ia.hpp"

JoueurIA::JoueurIA(const std::string& nom) : nom(nom), position(0) {}

void JoueurIA::avancer(int valeurDe) {
    // Logique de l'IA pour avancer
    position += valeurDe;
    if (position > 50) {
        position = 50;
    }
}

const std::string& JoueurIA::getNom() const {
    return nom;
}

int JoueurIA::getPosition() const {
    return position;
}
