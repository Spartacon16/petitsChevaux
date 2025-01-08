#include <SFML/Graphics.hpp>
#include "menu.hpp"
#include "music.hpp"


int main(void) {

    std::string folderPath = "../musique ambiance";
    // Lancer la fonction en parallèle
    std::thread musicThread(playMusicFromFolder, folderPath);
    std::string soundPath = "../bruits";

    sf::RenderWindow window(sf::VideoMode(800, 600), "Petits Chevaux");
    window.setFramerateLimit(60);
    Menu menu(window);
    
    while (window.isOpen()) {
            menu.handleEvents(window);
            menu.render(window);
       
    }

    return 0;
}
