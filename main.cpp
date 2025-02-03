#include <SFML/Graphics.hpp>
#include <thread>
#include "menu.hpp"
#include "music.hpp"
#include "jeu.hpp"

int main()
{
    // Lance la musique en fond (dossier ../musique ambiance)
    std::string folderPath = "../musique ambiance";
    std::thread musicThread(playMusicFromFolder, folderPath);
    musicThread.detach(); // pour que le thread tourne en arrière-plan

    sf::RenderWindow window(sf::VideoMode(800, 600), "Petits Chevaux");
    window.setFramerateLimit(60);
    
    Menu menu(window);
    Jeu jeu(window, menu.font,
        menu.getClickSound(), menu.getCriSound(),
        menu.getDiceSound(), menu.getGameboardSounds());

    // Boucle principale
    while (window.isOpen())
    {
        // Tant que le menu n'a pas switché en mode "GAME_RUNNING", on affiche le menu :
        if (menu.getCurrentState() != GAME_RUNNING)
        {
            menu.handleEvents(window);
            window.clear();
            menu.render(window);

            // Si l'utilisateur vient de cliquer sur "Start" en PLAYER_SELECTION,
            // le menu passera l'état à GAME_RUNNING
        }
        else
        { 
            // On récupère la liste des joueurs choisis
            auto& playersSelected = menu.getPlayers();

            // On transmet ces joueurs à la classe Jeu
            jeu.setPlayers(playersSelected);

            // Lancement du jeu
            jeu.run(window);

            // Lorsque la boucle jeu s'arrêtera (isRunning = false), on repasse au MAIN_MENU
            menu.setCurrentState(MAIN_MENU);
        }
    }

    return 0;
}
