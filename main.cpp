#include <SFML/Graphics.hpp>
#include "menu.hpp"

int main(void) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Petits Chevaux");
    window.setFramerateLimit(60);
    Menu menu(window);
    // PlayerSelectionMenu playerSelectionMenu;
    

    while (window.isOpen()) {
        // Boucle principale du menu
        if (!menu.isPlayerSelectionActive) {
            menu.handleEvents(window);
            menu.render(window);
        // } else {
        //     playerSelectionMenu.handleEvents(window);
        //     playerSelectionMenu.render(window);
        }
        
    }

    return 0;
}
