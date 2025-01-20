#include <SFML/Graphics.hpp>
#include "menu.hpp"
#include "music.hpp"
#include "jeu.hpp"

int main(void) {

    std::string folderPath = "../musique ambiance";
    // Lancer la fonction en parallèle
    std::thread musicThread(playMusicFromFolder, folderPath);
    std::string soundPath = "../bruits";

    sf::RenderWindow window(sf::VideoMode(800, 600), "Petits Chevaux");
    window.setFramerateLimit(60);
    Menu menu(window);
    Jeu jeu(window);
    bool gojouer=0,initjeu=0;
    while (window.isOpen()) {
        if(gojouer==0){
            menu.handleEvents(window);
            menu.render(window);
        }else if (initjeu==1){
            sf::RenderWindow window(sf::VideoMode(900, 675), "GAME");
            initjeu=0;
        }else{
            jeu.run(window);
        }
    }

    return 0;
}
