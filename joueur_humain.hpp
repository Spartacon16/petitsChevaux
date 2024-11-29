#ifndef JOUEUR_HUMAIN_HPP
#define JOUEUR_HUMAIN_HPP

#include "joueur_interface.hpp"
#include <string>

class JoueurHumain : public JoueurInterface {
public:
    JoueurHumain(const std::string& nom);
    void avancer(int valeurDe) override;
    const std::string& getNom() const override;
    int getPosition() const override;

private:
    std::string nom;
    int position;
};

#endif // JOUEUR_HUMAIN_HPP
