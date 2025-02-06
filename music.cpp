#include "music.hpp"

void playMusicFromFolder(const std::string& folderPath) {
    std::vector<std::string> musicFiles;

    // Parcours des fichiers dans le dossier
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();
            musicFiles.push_back(filePath);
        }
    }

    if (musicFiles.empty()) {
        std::cerr << "Aucun fichier audio trouve dans le dossier." << std::endl;
        return;
    }

    // Initialiser le generateur de nombres aleatoires
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    while (true) {
        // Choisir un fichier aleatoire a chaque fois
        int randomIndex = std::rand() % musicFiles.size();
        std::string selectedMusic = musicFiles[randomIndex];

        std::cout << "Musique selectionnee : " << selectedMusic << std::endl;

        sf::Music music;
        if (music.openFromFile(selectedMusic)) {
            music.setVolume(30.f); // Volume faible
            music.play();
            while (music.getStatus() == sf::Music::Playing) {
                sf::sleep(sf::milliseconds(100));
            }
        } else {
            std::cerr << "Impossible de lire le fichier : " << selectedMusic << std::endl;
        }

        // Attendre un certain temps avant de jouer a nouveau une musique pour eviter une repetition rapide
        sf::sleep(sf::seconds(10)); // Attendre 10 secondes avant de jouer a nouveau une musique
    }
}
