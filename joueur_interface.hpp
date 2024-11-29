#ifndef JOUEUR_INTERFACE_HPP
#define JOUEUR_INTERFACE_HPP

#include <string>

class JoueurInterface {
public:
    virtual ~JoueurInterface() = default;
    virtual void avancer(int valeurDe) = 0;
    virtual const std::string& getNom() const = 0;
    virtual int getPosition() const = 0;
};

#endif // JOUEUR_INTERFACE_HPP
