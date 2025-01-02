#include <SFML/Graphics.hpp>
#include "menu.hpp"
#include "jeu.hpp"

int main() {
    // Créer et afficher le menu
    Menu menu;
    menu.run();

    // Après la fermeture du menu, démarrer le jeu
    Jeu jeu;
    jeu.run();

    return 0;
}
