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
        std::cerr << "Aucun fichier audio trouvé dans le dossier." << std::endl;
        return;
    }

    // Initialiser le générateur de nombres aléatoires
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    while (true) {
        // Choisir un fichier aléatoire à chaque fois
        int randomIndex = std::rand() % musicFiles.size();
        std::string selectedMusic = musicFiles[randomIndex];

        std::cout << "Musique sélectionnée : " << selectedMusic << std::endl;

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

        // Attendre un certain temps avant de jouer à nouveau une musique pour éviter une répétition rapide
        sf::sleep(sf::seconds(10)); // Attendre 10 secondes avant de jouer à nouveau une musique
    }
}
