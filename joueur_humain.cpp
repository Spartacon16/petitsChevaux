#include "joueur_humain.hpp"

JoueurHumain::JoueurHumain(const std::string& nom) : nom(nom), position(0) {}

void JoueurHumain::avancer(int valeurDe) {
    position += valeurDe;
    if (position > 50) {
        position = 50;
    }
}

const std::string& JoueurHumain::getNom() const {
    return nom;
}

int JoueurHumain::getPosition() const {
    return position;
}
