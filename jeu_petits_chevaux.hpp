#ifndef JEU_PETIT_CHEVAUX_HPP
#define JEU_PETIT_CHEVAUX_HPP

#include <vector>
#include <memory>
#include "joueur_interface.hpp"

class JeuPetitsChevaux {
public:
    JeuPetitsChevaux();
    void ajouterJoueur(std::shared_ptr<JoueurInterface> joueur);
    void lancerDe();
    void afficherPositions() const;
    bool estTermine() const;

private:
    std::vector<std::shared_ptr<JoueurInterface>> joueurs;
    int joueurActuel;
    const int positionMax = 50;
};

#endif // JEU_PETIT_CHEVAUX_HPP
