#ifndef JEU_HPP
#define JEU_HPP

#include <SFML/Graphics.hpp>
#include "graphique.hpp"

class Jeu {
public:
    Jeu();
    void run();  // Méthode pour démarrer et gérer la boucle du jeu

private:
    void handleEvents();
    void update();
    void render();
    
    sf::RenderWindow window;
    Plateau plateau;  // Plateau de jeu
    std::vector<Pion> pions;  // Liste des pions sur le plateau

    bool isRunning;  // Indique si le jeu est en cours
};

#endif // JEU_HPP
