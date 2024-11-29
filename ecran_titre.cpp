#include <iostream>
#include "jeu_petits_chevaux.hpp"
#include "joueur_humain.hpp"
#include "joueur_ia.hpp"

void afficherEcranTitre(JeuPetitsChevaux& jeu) {
    int nbJoueurs;
    std::cout << "Bienvenue au jeu des petits chevaux!\n";
    std::cout << "Entrez le nombre de joueurs (1-4): ";
    std::cin >> nbJoueurs;

    for (int i = 1; i <= nbJoueurs; ++i) {
        std::string typeJoueur;
        std::string nom;
        std::cout << "Joueur " << i << ": Humain ou IA? (h/i): ";
        std::cin >> typeJoueur;
        std::cout << "Entrez le nom du joueur " << i << ": ";
        std::cin >> nom;

        if (typeJoueur == "h" || typeJoueur == "H") {
            jeu.ajouterJoueur(std::make_shared<JoueurHumain>(nom));
        } else {
            jeu.ajouterJoueur(std::make_shared<JoueurIA>(nom));
        }
    }
}
