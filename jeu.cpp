#include "jeu.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <cmath>

// Constructeur
Jeu::Jeu(sf::RenderWindow& window, const sf::Font& font,
         const sf::Sound& clickSound, const sf::Sound& criSound,
         const sf::Sound& diceSound, const std::vector<sf::Sound>& gameboardSounds)
    : window(window), font(font), isRunning(false), joueurActuel(0), valeurDe(0),
      clickSound(clickSound), criSound(criSound), diceSound(diceSound), gameboardSounds(gameboardSounds) {
    // Initialisation de la graine pour les nombres aleatoires
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Charger la texture du plateau
    if (!plateauTexture.loadFromFile("../table_jeu.png")) {
        std::cerr << "Erreur : Impossible de charger table_jeu.png\n";
    }
    plateauSprite.setTexture(plateauTexture);

    // Redimensionner le plateau à 600x600
    sf::Vector2u texSize = plateauTexture.getSize(); // Taille originale (par ex. 225×225)
    float scaleX = 600.f / texSize.x;
    float scaleY = 600.f / texSize.y;
    plateauSprite.setScale(scaleX, scaleY);

    // Charger la texture des pions
    if (!pionTexture.loadFromFile("../Pion.png")) {
        std::cerr << "Erreur : Impossible de charger Pion.png\n";
    }

    // Taille d'un carreau
    const float carreauPx = 50.f;

    // Calcul de l'échelle pour ajuster le pion à un carreau
    pionScale = carreauPx / 800.f; // 800 px correspond à la taille de l'image Pion.png

    // Positions de départ pour chaque joueur (zones de départ, en carré)
    startingPositions = {
        {100.f, 100.f}, // Jaune
        {450.f, 450.f}, // Rouge
        {100.f, 450.f}, // Vert
        {450.f, 100.f}  // Bleu
    };

    // Couleurs des joueurs
    playerColors = {
        sf::Color(255, 255, 153),   // Jaune pastel
        sf::Color(255, 102, 102),   // Rouge pastel
        sf::Color(102, 255, 102),   // Vert pastel
        sf::Color(102, 178, 255)    // Bleu pastel
    };

    // Bouton "Lancer le de"
    boutonLancerDe.setSize(sf::Vector2f(150.f, 50.f));
    boutonLancerDe.setPosition(610.f, 300.f);
    boutonLancerDe.setFillColor(sf::Color::White);
    boutonLancerDe.setOutlineThickness(2);
    boutonLancerDe.setOutlineColor(sf::Color::Black);

    // Texte du bouton
    texteLancerDe.setFont(font);
    texteLancerDe.setString("Lancer le de");
    texteLancerDe.setCharacterSize(20);
    texteLancerDe.setFillColor(sf::Color::Black);
    texteLancerDe.setPosition(boutonLancerDe.getPosition().x + 10, boutonLancerDe.getPosition().y + 10);

    // Texte des actions
    texteActions.setFont(font);
    texteActions.setCharacterSize(20);
    texteActions.setFillColor(sf::Color::Black);
    texteActions.setPosition(610.f, 50.f);
    texteActions.setString("Joueur: J1\n=> Lancez le de");
    // Pion du joueur actuel (affiché à droite du texte des actions)
    txt_pionJoueurActuel.setTexture(pionTexture);
    txt_pionJoueurActuel.setOrigin(400.f, 400.f); // Centrer le sprite
    txt_pionJoueurActuel.setScale(pionScale, pionScale); // Même échelle que les autres pions
    txt_pionJoueurActuel.setColor(playerColors[joueurActuel]); // Couleur du premier joueur
    txt_pionJoueurActuel.setPosition(texteActions.getPosition().x + 150.f, texteActions.getPosition().y - 10.f);

    setupPlateau();
    diceRolled = false;
    attenteValidation = false;

}

// Configurer les joueurs et leurs pions
void Jeu::setPlayers(const std::vector<PlayerInfo>& playersSelected) {
    playersInGame = playersSelected;
    playerPions.clear(); // Reinitialiser les pions pour tous les joueurs

    for (size_t i = 0; i < playersInGame.size(); ++i) {
        std::vector<PionInfo> pionsJoueur; // 4 pions par joueur

        for (int j = 0; j < 4; ++j) {
            PionInfo pion;
            pion.sprite.setTexture(pionTexture);
            pion.sprite.setOrigin(400.f, 400.f); // Centrer le sprite
            pion.sprite.setScale(pionScale, pionScale); // Ajuster la taille
            pion.sprite.setColor(playerColors[i]);

            // Position des pions dans la prison
            pion.startPosition = {
                startingPositions[i].x + (j % 2) * 50.f,
                startingPositions[i].y + (j / 2) * 50.f
            };
            pion.sprite.setPosition(pion.startPosition);
            pion.isOut = false; // Initialisation des pions comme étant dans la prison
            pionsJoueur.push_back(pion);
        }

        playerPions.push_back(pionsJoueur);
    }
}

// Gérer les événements
void Jeu::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (boutonLancerDe.getGlobalBounds().contains(mousePos)) {
                if (attenteValidation) {
                    // Désactiver "OK" et passer au joueur suivant
                    attenteValidation = false;
                    texteLancerDe.setString("Lancer le de");
                    passerAuJoueurSuivant();
                } else if (!diceRolled) {
                    lancerDe();
                }
            } else {
                gererClicPion(mousePos);
            }
        }
    }
}

// Gérer le clic sur un pion
void Jeu::gererClicPion(const sf::Vector2f& mousePos) {
    for (PionInfo& pion : playerPions[joueurActuel]) {
        if (pion.sprite.getGlobalBounds().contains(mousePos)) {
            if (valeurDe > 0 && pion.isOut) { // Déplacement si le pion est déjà sorti
                avancerPion(pion);
                passerAuJoueurSuivant();
                return;
            }
            if (valeurDe == 6 && !pion.isOut) { // Sortir un pion si on fait 6
                sortirPion(pion);
                passerAuJoueurSuivant();
                return;
            }
        }
    }
}

// Lancer le dé
void Jeu::lancerDe() {
    // Jouer le son du dé
    diceSound.play();
    valeurDe = rand() % 6 + 1;
    std::string message = "Joueur: " + playersInGame[joueurActuel].name + "\nDe: " + std::to_string(valeurDe);
    std::cout << "Valeur du de: " << valeurDe << std::endl;
    // Ajouter la couleur du pion à côté du nom du joueur
    sf::Text playerName;
    playerName.setFont(font);
    playerName.setString(playersInGame[joueurActuel].name);
    playerName.setFillColor(playerColors[joueurActuel]);
    playerName.setCharacterSize(20);
    playerName.setPosition(texteActions.getPosition().x, texteActions.getPosition().y + 30);

    texteActions.setString(message);
    diceRolled = true; // Indiquer que le dé a été lancé pour ce tour

    if (valeurDe == 6 && std::none_of(playerPions[joueurActuel].begin(), playerPions[joueurActuel].end(), [](const PionInfo& pion) { return pion.isOut; })) {
        if (std::any_of(playerPions[joueurActuel].begin(), playerPions[joueurActuel].end(), [](const PionInfo& pion) { return !pion.isOut; })) {
            message += "\n=> Sortez un pion.";
            texteActions.setString(message);
        } else {
            message += "\n=> Sortez un pion \nou avancez.";
            texteActions.setString(message);
        }
        
    } else if (std::none_of(playerPions[joueurActuel].begin(), playerPions[joueurActuel].end(), [](const PionInfo& pion) { return pion.isOut; })) {
        message += "\n=> Joueur suivant...";
        texteActions.setString(message);
        // Activer le mode "OK"
        attenteValidation = true;
        texteLancerDe.setString("OK");
        return;
    } 
    else {
        message += "\n=> Avancez un pion.";
        texteActions.setString(message);
    }
}

// Sortir un pion
void Jeu::sortirPion(PionInfo& pion) {
     // Vérifier si un pion de la même couleur est déjà sur la case cible
    int nouvelleCase = (valeurDe) % cases.size();
    for (const PionInfo& autrePion : playerPions[joueurActuel]) {
        if (&autrePion != &pion && autrePion.isOut && autrePion.sprite.getPosition() == cases[nouvelleCase].position) {
            // Empêcher le déplacement
            std::string message = "Joueur: " + playersInGame[joueurActuel].name + "\nImpossible de passer\n meme couleur!";
            texteActions.setString(message);
            std::cout << message << std::endl;
            return; // Bloquer le mouvement
        }
    }
    if (joueurActuel == 0) {
        pion.sprite.setPosition(getCasePosition(0, joueurActuel)); // Yellow start
    } else if (joueurActuel == 3) {
        pion.sprite.setPosition(getCasePosition(14, joueurActuel)); // Blue start
    } else if (joueurActuel == 1) {
        pion.sprite.setPosition(getCasePosition(28, joueurActuel)); // Red start
    } else if (joueurActuel == 2) {
        pion.sprite.setPosition(getCasePosition(42, joueurActuel)); // Green start
    }
    pion.isOut = true;

   
    // Jouer un son de plateau aléatoire
    int sonIndex = rand() % gameboardSounds.size();
    gameboardSounds[sonIndex].play();

    std::string message = "Joueur: " + playersInGame[joueurActuel].name + "\nPion sorti !";
    texteActions.setString(message);   
    // Log the position to the console
    std::cout << "Pion sorti à la position: (" << pion.startPosition.x << ", " << pion.startPosition.y << ")" << std::endl;
}

// Avancer un pion
void Jeu::avancerPion(PionInfo& pion) {
    // Trouver la position actuelle du pion
    sf::Vector2f positionActuelle = pion.sprite.getPosition();
    int caseActuelle = trouverIndexCase(positionActuelle);
    int nouvelleCase = (caseActuelle + valeurDe) % cases.size();
    int caseFinale;
    switch (joueurActuel) {
        case 0: caseFinale = 55; break; // Jaune
        case 1: caseFinale = 27; break; // Rouge
        case 2: caseFinale = 41; break; // Vert
        case 3: caseFinale = 13; break; // Bleu
        default: caseFinale = -1; break;
    }
    // 📌 Empêcher d'avancer si la case dépasse la case finale
    if (caseActuelle < caseFinale && nouvelleCase > caseFinale) {
        std::string message = "Joueur: " + playersInGame[joueurActuel].name + 
                              "\nVous ne pouvez pas dépasser votre case finale.";
        texteActions.setString(message);
        std::cout << message << std::endl;
        return;
    }
    // 📌 Vérifier si le pion est déjà sur la case finale et doit attendre un 6
    

    if (caseActuelle == caseFinale) {
        if (valeurDe == 6) {
            // 🏆 Victoire !
            std::string message = "🎉 " + playersInGame[joueurActuel].name + " a gagné ! 🎉";
            texteActions.setString(message);
            std::cout << message << std::endl;
            isRunning = false; // Fin du jeu
        } else {
            // 📌 Afficher un message pour dire qu'il faut un 6
            std::string message = "Joueur: " + playersInGame[joueurActuel].name + 
                                  "\nVous devez faire un 6 pour gagner!";
            texteActions.setString(message);
            std::cout << message << std::endl;
        }
        return;
    }
    // Vérifier si un pion de la même couleur est déjà sur la case cible
    for (const PionInfo& autrePion : playerPions[joueurActuel]) {
        if (&autrePion != &pion && autrePion.isOut && autrePion.sprite.getPosition() == cases[nouvelleCase].position) {
            // Empêcher le déplacement
            std::string message = "Joueur: " + playersInGame[joueurActuel].name + "\nImpossible de passer\n meme couleur!";
            texteActions.setString(message);
            std::cout << message << std::endl;
            return; // Bloquer le mouvement
        }
    }
    // Animation du déplacement du pion
    sf::Vector2f startPosition = pion.sprite.getPosition();
    sf::Vector2f endPosition = cases[nouvelleCase].position;
    sf::Clock clock;
    while (clock.getElapsedTime().asSeconds() < 1.0f) {
        float progress = clock.getElapsedTime().asSeconds();
        pion.sprite.setPosition(startPosition + progress * (endPosition - startPosition));
        render(window);
    }
    pion.sprite.setPosition(endPosition);

    // Jouer un son de plateau aléatoire
    int sonIndex = rand() % gameboardSounds.size();
    gameboardSounds[sonIndex].setVolume(300.f);
    gameboardSounds[sonIndex].play();

    std::string message = "Joueur: " + playersInGame[joueurActuel].name + "\nPion avance !";
    texteActions.setString(message);// Log the new position to the console
    std::cout << "Pion avancé à la position: (" << cases[nouvelleCase].position.x << ", " << cases[nouvelleCase].position.y << ")" << std::endl;

    // Vérifier si un pion adverse est sur la nouvelle case ou a été sauté
    for (auto& pionsJoueur : playerPions) {
        for (auto& autrePion : pionsJoueur) {
            int caseAutrePion = trouverIndexCase(autrePion.sprite.getPosition());
            if (autrePion.isOut && &autrePion != &pion && 
                (autrePion.sprite.getPosition() == pion.sprite.getPosition() || 
                (caseAutrePion > caseActuelle && caseAutrePion <= nouvelleCase))) {
                // Remettre le pion adverse dans sa prison
                autrePion.sprite.setPosition(autrePion.startPosition);
                autrePion.isOut = false;
                std::cout << "Pion adverse renvoyé à la prison: (" << autrePion.startPosition.x << ", " << autrePion.startPosition.y << ")" << std::endl;
            }
        }
    }
}

// Passer au joueur suivant
void Jeu::passerAuJoueurSuivant() {
    if (!playersInGame.empty()) {
        joueurActuel = (joueurActuel + 1) % playersInGame.size();
        valeurDe = 0; // Réinitialiser le dé
        diceRolled=false; // Réinitialiser le dé lancé
        std::string message = "Joueur: " + playersInGame[joueurActuel].name + "\n=> Lancez le de";
        texteActions.setString(message);
        std::cout << message << std::endl;
        // 🔄 Mettre à jour l'apparence du pion actuel
        txt_pionJoueurActuel.setColor(playerColors[joueurActuel]);
    }
}

// Afficher le jeu
void Jeu::render(sf::RenderWindow& window) {
    window.clear(sf::Color::White); // Fond blanc

    // Dessiner le plateau
    window.draw(plateauSprite);

    // Dessiner les pions
    for (const auto& pionsJoueur : playerPions) {
        for (const auto& pion : pionsJoueur) {
            window.draw(pion.sprite);
        }
    }

    // Dessiner le bouton et le texte
    window.draw(boutonLancerDe);
    window.draw(texteLancerDe);
    window.draw(texteActions);
    window.draw(txt_pionJoueurActuel);
    window.display();
}

// Configurer le plateau
void Jeu::setupPlateau() {
    generateParcours();
    // Ajouter la case "Victoire" au centre
    cases.emplace_back(sf::Vector2f(300.f, 300.f), "Victoire",0);
    cases.emplace_back(sf::Vector2f(300.f, 300.f), "Victoire",1);
    cases.emplace_back(sf::Vector2f(300.f, 300.f), "Victoire",2);
    cases.emplace_back(sf::Vector2f(300.f, 300.f), "Victoire",3);
}

// Generer les cases du parcours
void Jeu::generateParcours() {
    // Facteur d'agrandissement du plateau
    const float scaleFactor = 600.f / 225.f;
    const float delta = 13.5f * scaleFactor;
    const sf::Vector2f yellowStart(16.5f * scaleFactor, 98.5f * scaleFactor);
    const sf::Vector2f blueStart(16.5f * scaleFactor+ 8*delta, 98.5f * scaleFactor - 6 * delta);
    const sf::Vector2f redStart(16.5f * scaleFactor + 14 * delta, 98.5f * scaleFactor + 2 * delta);
    const sf::Vector2f greenStart(16.5f * scaleFactor + 6 * delta, 98.5f * scaleFactor + 8 * delta);

     // Cases du parcours pour chaque joueur
     int i;
    for (i = 0; i < 14; ++i) {
        // 🟡 Cases Jaunes +1 bleu
        if (i < 7) {
            cases.push_back({yellowStart + sf::Vector2f(i * delta, 0), "Parcours", 0});
        } else if (i < 13) {
            cases.push_back({yellowStart + sf::Vector2f(6 * delta, -(i - 6) * delta), "Parcours", 0});
        } else {
            cases.push_back({yellowStart + sf::Vector2f(7 * delta, -6 * delta), "Parcours", 1});
        }
    }
    for (i = 0; i < 14; ++i){
        // 🔵 Cases Bleues +1 rouge
        if (i < 7) {
            cases.push_back({blueStart + sf::Vector2f(0, i * delta), "Parcours", 1});
        } else if (i < 13) {
            cases.push_back({blueStart + sf::Vector2f((i - 6) * delta, 6 * delta), "Parcours", 1});
        } else {
            cases.push_back({blueStart + sf::Vector2f(6 * delta, 7 * delta), "Parcours", 2});
        }
    }
    for (i = 0; i < 14; ++i){

        // 🔴 Cases Rouges +1 vert
        if (i < 7) {
            cases.push_back({redStart + sf::Vector2f(-i * delta, 0), "Parcours", 2});
        } else if (i < 13) {
            cases.push_back({redStart + sf::Vector2f(-6 * delta, (i - 6) * delta), "Parcours", 2});
        } else {
            cases.push_back({redStart + sf::Vector2f(-7 * delta, 6 * delta), "Parcours", 3});
        }
    }
    for (i = 0; i < 14; ++i){

        // 🟢 Cases Vertes +1 jaune
        if (i < 7) {
            cases.push_back({greenStart + sf::Vector2f(0, -i * delta), "Parcours", 3});
        } else if (i < 13) {
            cases.push_back({greenStart + sf::Vector2f(-(i - 6) * delta,- 6 * delta), "Parcours", 3});
        } else {
            cases.push_back({greenStart + sf::Vector2f(-6 * delta, -7 * delta), "Parcours", 0});
        }
    }
}

// Lancer la boucle du jeu
void Jeu::run(sf::RenderWindow& window) {
    isRunning = true;

    while (window.isOpen() && isRunning) {
        handleEvents(window);
        update();
        render(window);
    }
}

// Mettre à jour la logique du jeu (placeholder pour futur ajout)
void Jeu::update() {
    // Ici, ajoutez les règles du jeu ou la logique de déplacement
}

sf::Vector2f Jeu::getCasePosition(int caseIndex, int joueur) {
    // Exemple : Retourne la position de la case sur le plateau
    if (caseIndex < 0 || caseIndex >= static_cast<int>(cases.size())) {
        return sf::Vector2f(0.f, 0.f); // Valeur de secours
    }
    return cases[caseIndex].position;
}

int Jeu::trouverIndexCase(const sf::Vector2f& position) {
    int bestIndex = -1;
    float minDist = std::numeric_limits<float>::max();

    for (size_t i = 0; i < cases.size(); ++i) {
        float distance = std::hypot(cases[i].position.x - position.x, cases[i].position.y - position.y);
        if (distance < minDist) {
            minDist = distance;
            bestIndex = static_cast<int>(i);
        }
    }
    return bestIndex;
}
