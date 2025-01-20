#ifndef JEU_HPP
#define JEU_HPP

#include <SFML/Graphics.hpp>
#include "jeu.hpp"
#include <vector>
class Pion {
public:
    enum class Couleur { Rouge, Vert, Bleu, Jaune };

    Pion(Couleur couleur, sf::Vector2f position);
    void afficher(sf::RenderWindow& window);
    void setPosition(sf::Vector2f newPosition);

private:
    Couleur couleur;
    sf::Vector2f position;
    sf::CircleShape cercle; 
};

class Plateau {
public:
    Plateau(int largeur, int hauteur);
    void ajouterPion(int x, int y, Pion pion);
    void afficher(sf::RenderWindow& window);

private:
    int largeur, hauteur;
    float caseSize;
    std::vector<std::vector<Pion>> pions; 
};

class Jeu {
public:
    Jeu(sf::RenderWindow& window);
    void run(sf::RenderWindow& window);  // Méthode pour démarrer et gérer la boucle du jeu

private:
    void handleEvents(sf::RenderWindow& window);
    void update();
    void render(sf::RenderWindow& window);
    
    sf::RenderWindow &window;
    Plateau plateau;  // Plateau de jeu
    std::vector<Pion> pions;  // Liste des pions sur le plateau

    bool isRunning;  // Indique si le jeu est en cours
};


#endif // JEU_HPP
