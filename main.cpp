#include "jeu_petits_chevaux.hpp"
#include "ecran_titre.cpp"

int main() {
    JeuPetitsChevaux jeu;
    afficherEcranTitre(jeu);

    while (!jeu.estTermine()) {
        std::cout << "1. Lancer le dé\n";
        std::cout << "2. Quitter\n";
        int choix;
        std::cin >> choix;

        switch (choix) {
            case 1:
                jeu.lancerDe();
                jeu.afficherPositions();
                break;
            case 2:
                return 0;
            default:
                std::cout << "Choix invalide.\n";
                break;
        }
    }

    return 0;
}
