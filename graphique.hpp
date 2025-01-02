#ifndef GRAPHIQUE_HPP
#define GRAPHIQUE_HPP

#include <SFML/Graphics.hpp>
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

#endif // GRAPHIQUE_HPP
