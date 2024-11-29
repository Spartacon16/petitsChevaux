#ifndef JOUEUR_IA_HPP
#define JOUEUR_IA_HPP

#include "joueur_interface.hpp"
#include <string>

class JoueurIA : public JoueurInterface {
public:
    JoueurIA(const std::string& nom);
    void avancer(int valeurDe) override;
    const std::string& getNom() const override;
    int getPosition() const override;

private:
    std::string nom;
    int position;
};

#endif // JOUEUR_IA_HPP
