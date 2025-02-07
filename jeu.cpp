#include "jeu.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <cmath>

// Constructeur
Jeu::Jeu(sf::RenderWindow& window, const sf::Font& font, const int& nmbpion,
         const sf::Sound& clickSound, const sf::Sound& criSound,
         const sf::Sound& diceSound, const std::vector<sf::Sound>& gameboardSounds)
    : window(window), font(font), isRunning(false), joueurActuel(0), valeurDe(0),nmbpion(4),
      clickSound(clickSound), criSound(criSound), diceSound(diceSound), gameboardSounds(gameboardSounds) {
    // Initialisation de la graine pour les nombres aleatoires
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Charger la texture du plateau
    if (!plateauTexture.loadFromFile("../table_jeu.png")) {
        std::cerr << "Erreur : Impossible de charger table_jeu.png\n";
    }
    plateauSprite.setTexture(plateauTexture);

    // Redimensionner le plateau a 600x600
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

    // Calcul de l'echelle pour ajuster le pion a un carreau
    pionScale = carreauPx / 800.f; // 800 px correspond a la taille de l'image Pion.png

    // Positions de depart pour chaque joueur (zones de depart, en carre)
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

    // Pion du joueur actuel (affiche a droite du texte des actions)
    txt_pionJoueurActuel.setTexture(pionTexture);
    txt_pionJoueurActuel.setOrigin(400.f, 400.f); // Centrer le sprite
    txt_pionJoueurActuel.setScale(pionScale, pionScale); // Meme echelle que les autres pions
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

        for (int j = 0; j < nmbpion; ++j) {
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
            pion.isOut = false; // Initialisation des pions comme etant dans la prison
            pion.pionId=i;   // id couleur du pion de jeu -1 si neutre, 0"Yellow",3"Blue",1"Red",2"Green"
   
            pionsJoueur.push_back(pion);
        }

        playerPions.push_back(pionsJoueur);
    }
}

// Gerer les evenements
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
                    // Desactiver "OK" et passer au joueur suivant
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
// // Gerer le clic sur un pion en autorisant une re-selection en cas d'erreur
// void Jeu::gererClicPion(const sf::Vector2f& mousePos) {
//     bool pionSelectionne = false;

//     for (PionInfo& pion : playerPions[joueurActuel]) {
//         if (pion.sprite.getGlobalBounds().contains(mousePos)) {
//             if (valeurDe > 0 && pion.isOut) { 
//                 // Si le pion est deja sorti, tenter de l'avancer
//                 if (avancerPion(pion)) { // AvancerPion retourne false si mouvement interdit
//                     passerAuJoueurSuivant();
//                 }
//                 return;
//             }

//             if (valeurDe == 6 && !pion.isOut) { 
//                 // Tenter de sortir le pion
//                 if (sortirPion(pion)) { // SortirPion retourne false si sortie interdite
//                     passerAuJoueurSuivant();
//                 }
//                 return;
//             }

//             pionSelectionne = true;
//         }
//     }

//     // Permettre au joueur de re-cliquer si aucun pion valide n'a ete selectionne
//     if (!pionSelectionne) {
//         std::string message ="Joueur: " + playersInGame[joueurActuel].name + "\nSelec invalide. \nCliquez sur un \n-pion disponible\n ou -""ok"" .";
//         std::cout << message << std::endl;
//     }
// }
void Jeu::gererClicPion(const sf::Vector2f& mousePos) {
    // Si le joueur n'a pas encore lance le de, on empeche de cliquer sur un pion
    if (!diceRolled) {
        std::string message = "Joueur: " + playersInGame[joueurActuel].name +                               
                              "\nLancez le de \navant de jouer.";
        texteActions.setString(message);
        std::cout << message << std::endl;
        return;
    }

    bool pionSelectionne = false;  
    PionInfo* pionChoisi = nullptr; 

    // Verifier si un pion a ete clique
    for (PionInfo& pion : playerPions[joueurActuel]) {
        if (pion.sprite.getGlobalBounds().contains(mousePos)) {
            pionSelectionne = true;
            pionChoisi = &pion;
            break;  // On arrete la boucle des qu'un pion valide est trouve
        }
    }

    // Si aucun pion valide n'est selectionne, afficher un message et permettre de re-choisir
    if (!pionSelectionne) {
        std::string message = "Joueur: " + playersInGame[joueurActuel].name + 
                              "\nDe: " + std::to_string(valeurDe) +
                              "\nSelec invalide.\nCliquez sur un \npion valide.\n ou ""ok"".";
        texteActions.setString(message);
        std::cout << message << std::endl;
        return;
    }

     // Verifier si le pion selectionne peut avancer
    bool peutAvancer = peutAvancerPion(*pionChoisi);  

    if (pionChoisi->isOut) {  
        if (peutAvancer) {
            if (avancerPion(*pionChoisi)) {  
                passerAuJoueurSuivant();  
            }
        } else {
            std::string message = "Joueur: " + playersInGame[joueurActuel].name + 
                                  "\nDe: " + std::to_string(valeurDe) +
                                  "\nCe pion est bloque.\nChoisissez un \nautre pion.";
            texteActions.setString(message);
            std::cout << message << std::endl;
            attenteValidation = true;
            texteLancerDe.setString("OK");
        }
    } 
    else if (valeurDe == 6) {  
        if (sortirPion(*pionChoisi)) { 
            passerAuJoueurSuivant();
        }
    } 
    else {
        std::string message = "Joueur: " + playersInGame[joueurActuel].name + 
                              "\nDe: " + std::to_string(valeurDe) +
                              "\nCe pion ne peut \npas bouger.\nChoisissez un \nautre pion.\n ou ""ok"".";
        texteActions.setString(message);
        std::cout << message << std::endl;
        attenteValidation = true;
        texteLancerDe.setString("OK");
    }
}




// Lancer le de
void Jeu::lancerDe() {
    // Jouer le son du de
    diceSound.play();
    valeurDe = rand() % 6 + 1;
    std::string message = "Joueur: " + playersInGame[joueurActuel].name + "\nDe: " + std::to_string(valeurDe);
    std::cout << "Valeur du de: " << valeurDe << std::endl;
    // Ajouter la couleur du pion a côte du nom du joueur
    sf::Text playerName;
    playerName.setFont(font);
    playerName.setString(playersInGame[joueurActuel].name);
    playerName.setFillColor(playerColors[joueurActuel]);
    playerName.setCharacterSize(20);
    playerName.setPosition(texteActions.getPosition().x, texteActions.getPosition().y + 30);

    texteActions.setString(message);
    diceRolled = true; // Indiquer que le de a ete lance pour ce tour

    bool auMoinsUnPionSorti = std::any_of(playerPions[joueurActuel].begin(), playerPions[joueurActuel].end(), 
                                      [](const PionInfo& pion) { return pion.isOut; });
   bool peutAvancer = false; // Initialisation en dehors de la boucle

    for (PionInfo& pion : playerPions[joueurActuel]) {
        if (peutAvancerPion(pion)) { 
            peutAvancer = true; // Des qu'un pion peut avancer, on met true
            break; // Pas besoin de continuer la boucle
        }
    }
    if (!auMoinsUnPionSorti && valeurDe == 6) {
        message += "\n=> Sortez un pion.";
    } else if (auMoinsUnPionSorti && valeurDe == 6) {
        message += "\n=> Sortez un pion \nou avancez.";
    } else if (auMoinsUnPionSorti && peutAvancer) {
        message += "\n=> Avancez un pion.";
    } else {
        message += "\nPions bloques, \nPassez votre tour.";
        texteActions.setString(message);
        attenteValidation = true;
        texteLancerDe.setString("OK");
        return;
    }

    texteActions.setString(message);
    
}
bool Jeu::peutAvancerPion(PionInfo& pion) {
    int caseActuelle = trouverIndexCase(pion.sprite.getPosition());
    int nouvelleCase = caseActuelle + 1; // En Final, toujours avancer d'une case

    // ✅ Si on est en zone "Final", avancer toujours de 1 case seulement
    if (cases[caseActuelle].type == "Final") {
        int finalStep = cases[caseActuelle].finalStep; // Étape actuelle
        int nouvelleStep = finalStep + 1; // Prochaine étape
        if (nouvelleStep > 6) {
            if( valeurDe == 6) {
                std::string message = "🎉 " + playersInGame[joueurActuel].name + " a gagné la partie ! 🎉";
                texteActions.setString(message);
                std::cout << message << std::endl;
                isRunning = false; // Arrêter le jeu
                return true;
            }
           }

        // ✅ Vérifier si la case suivante est occupée
        for (auto& pionsJoueur : playerPions) {
            for (auto& autrePion : pionsJoueur) {
                if (&autrePion != &pion && autrePion.isOut && autrePion.sprite.getPosition() == cases[nouvelleCase].position) {
                    return false; // La case est bloquée
                }
            }
        }
        return true; // ✅ Le pion peut avancer d'une case
    }

    // ✅ Si le pion n'est pas en Final, utiliser le comportement normal
    nouvelleCase = (caseActuelle + valeurDe) % cases.size();

    if (caseActuelle <= 55 && nouvelleCase > 55) {
        nouvelleCase = (nouvelleCase - 56); // Gestion du retour à 0
    }

    // Verifier si la case cible est libre
    sf::Vector2f sortiePosition;
    int caseAvantFinale;
    switch (joueurActuel) {
        case 0: sortiePosition = getCasePosition(0, joueurActuel);  caseAvantFinale = 55; break;
        case 1: sortiePosition = getCasePosition(28, joueurActuel);  caseAvantFinale = 27;break;
        case 2: sortiePosition = getCasePosition(42, joueurActuel);  caseAvantFinale = 41;break;
        case 3: sortiePosition = getCasePosition(14, joueurActuel); caseAvantFinale = 13; break;
        default: return false;
    }
    if (caseActuelle < caseAvantFinale && nouvelleCase > caseAvantFinale) {
        nouvelleCase = caseAvantFinale;
     for (auto& pionsJoueur : playerPions) {
        for (auto& autrePion : pionsJoueur) {
            if (&autrePion != &pion && autrePion.isOut && autrePion.sprite.getPosition() == cases[nouvelleCase].position) {
                return false; // Case bloquee
            }
            if ((!pion.isOut)&&(&autrePion != &pion && autrePion.isOut && autrePion.sprite.getPosition() == sortiePosition)) {
             return false; // Case bloquee
            }
        }
    }
    }
    return true; // Deplacement possible
}
bool Jeu::sortirPion(PionInfo& pion) {
    sf::Vector2f sortiePosition;
    switch (joueurActuel) {
        case 0: sortiePosition = getCasePosition(0, joueurActuel); break;
        case 1: sortiePosition = getCasePosition(28, joueurActuel); break;
        case 2: sortiePosition = getCasePosition(42, joueurActuel); break;
        case 3: sortiePosition = getCasePosition(14, joueurActuel); break;
        default: return false;
    }

    // Verifier si un pion est deja sur la case de sortie
    for (auto& pionsJoueur : playerPions) {
        for (auto& autrePion : pionsJoueur){
            if (&autrePion != &pion && autrePion.isOut && autrePion.sprite.getPosition() == sortiePosition) {
            std::string message = "Joueur: " + playersInGame[joueurActuel].name +
            "\nDe: " + std::to_string(valeurDe) + "\nSortie de prison \ndeja prise!";
            texteActions.setString(message);
            std::cout << message << std::endl;
            texteLancerDe.setString("OK");
            return false; // Permet au joueur de re-choisir
            }
        }
    }
    // Sortir le pion normalement
    pion.sprite.setPosition(sortiePosition);
    pion.isOut = true;

    // Jouer un son pour la sortie
    int sonIndex = rand() % gameboardSounds.size();
    gameboardSounds[sonIndex].setVolume(500.f);
    gameboardSounds[sonIndex].play();

    std::string message = "Joueur: " + playersInGame[joueurActuel].name + "\nPion sorti!";
    texteActions.setString(message);

    return true;
}


// Avancer un pion
bool Jeu::avancerPion(PionInfo& pion) {
    // Trouver la position actuelle du pion
    sf::Vector2f positionActuelle = pion.sprite.getPosition();
    int caseActuelle = trouverIndexCase(positionActuelle);
    int nouvelleCase = (caseActuelle + valeurDe) % cases.size();
    int caseAvantFinale;
    int caseFinalStart;
    bool depasseavantfinalejaune=false;
    switch (joueurActuel) {
        case 0: caseAvantFinale = 55; caseFinalStart = 56; break; // Jaune
        case 1: caseAvantFinale = 27; caseFinalStart =68; break; // Rouge
        case 2: caseAvantFinale = 41; caseFinalStart =74; break; // Vert
        case 3: caseAvantFinale = 13; caseFinalStart =62 ; break; // Bleu
        default: return false;
    }
     // ✅ Empecher d'aller au-dela de la case avant finale sauf si en zone finale
    if (caseActuelle < caseAvantFinale && nouvelleCase > caseAvantFinale) {
        nouvelleCase = caseAvantFinale;
        std::string message = "Joueur: " + playersInGame[joueurActuel].name + 
                            "\nVous etes arrive a votre case finale!";
                            // Animation du deplacement du pion
    sf::Vector2f startPosition = pion.sprite.getPosition();
    sf::Vector2f endPosition = cases[nouvelleCase].position;
    sf::Clock clock;
    while (clock.getElapsedTime().asSeconds() < 1.0f) {
        float progress = clock.getElapsedTime().asSeconds();
        pion.sprite.setPosition(startPosition + progress * (endPosition - startPosition));
        render(window);
    }
    pion.sprite.setPosition(endPosition);
        texteActions.setString(message);
        std::cout << message << std::endl;
        return true; // Ne peut pas aller plus loin
    }

    // ✅ Gestion du depassement de 55 pour revenir a 0 correctement
    if (caseActuelle <= 55 && nouvelleCase > 55 ) {
        nouvelleCase = (nouvelleCase - 56); // On revient a 0 et on continue
        depasseavantfinalejaune=true;
    }

    // ✅ Vérification des pions de la meme couleur en avant, uniquement sur le chemin réel
    for (const PionInfo& autrePion : playerPions[joueurActuel]) {   
        int caseAutrePion = trouverIndexCase(autrePion.sprite.getPosition());

        bool bloqueMouvement = false; // Initialisation en dehors de la boucle
        // ✅ Cas 1 : Si on est dans un déplacement normal, vérifier si un pion de la meme couleur bloque
        if (caseActuelle < nouvelleCase && caseAutrePion > caseActuelle && caseAutrePion < nouvelleCase) {
           bloqueMouvement= true; 
        }

        // ✅ Cas 2 : Si on dépasse 55 et revient à 0, il faut aussi vérifier les pions après 55
        if (depasseavantfinalejaune && (caseAutrePion <= nouvelleCase || caseAutrePion > caseActuelle)) {
            bloqueMouvement = true;
        }

        if (bloqueMouvement && &autrePion != &pion && autrePion.isOut) {
            std::string message = "Joueur: " + playersInGame[joueurActuel].name + 
                                "\nMouv bloque!\nUn de vos \npions est devant.";
            texteActions.setString(message);
            std::cout << message << std::endl;
            return false; // Mouvement bloqué
        }
    }
  // ✅ Vérification des pions adverses à capturer
    for (auto& pionsJoueur : playerPions) {
        for (auto& autrePion : pionsJoueur) {
            if (cases[caseActuelle].type != "Final" || 
                (cases[caseActuelle].type != "Avantfinal" && autrePion.pionId != pion.pionId)) {

                int caseAutrePion = trouverIndexCase(autrePion.sprite.getPosition());

                // 📌 Capture uniquement si on **passe après** le pion adverse !
                bool doitCapturer = false;

                // ✅ Cas normal : on dépasse un pion adverse sans retour à 0
                if (caseAutrePion >= caseActuelle && caseAutrePion + 1 <= nouvelleCase) {
                    doitCapturer = true;
                }

                // ✅ Cas spécial : si on repasse par 0, on doit aussi vérifier les cases 0-55
                if (depasseavantfinalejaune && (caseAutrePion + 1 <= nouvelleCase || caseAutrePion > caseActuelle)) {
                    doitCapturer = true;
                }

                // 🚨 Vérification spéciale pour éviter la capture dans le cas "Avantfinal -> Final"
                if (cases[caseActuelle].type == "Avantfinal" && cases[caseAutrePion].type == "Parcours" 
                    && caseAutrePion == caseActuelle + 1 && nouvelleCase >= 56) {
                    continue; // On saute cette itération pour ne pas capturer le pion
                }

                // 📌 Vérifier que le pion capturé n'est pas dans la zone finale
                if (doitCapturer && autrePion.isOut && &autrePion != &pion && cases[caseAutrePion].type != "Final") {
                    autrePion.sprite.setPosition(autrePion.startPosition);
                    autrePion.isOut = false;
                    std::cout << "🔥 Pion adverse renvoyé en prison ! (Case " << caseAutrePion << ")" << std::endl;
                }
            }
        }
    }


    // ✅ Cas 1 : Si le pion est sur la case avant finale, il doit entrer en "FinalStep = 1"  avec un 1 ou plus au de
    if (caseActuelle == caseAvantFinale) {
        if (valeurDe >= 1) {
            nouvelleCase = caseFinalStart; // Aller en FinalStep 1
        } else {
            std::string message = "Joueur: " + playersInGame[joueurActuel].name + 
                                  "\nDe: " + std::to_string(valeurDe) +
                                  "\nVous devez \nfaire 1 ou \nplus pour \nentrer dans la \nzone finale!";
            texteActions.setString(message);
            std::cout << message << std::endl;
            return false; // Ne peut pas avancer
        }
    }

   
    // ✅ Cas 2 : Si le pion est déjà en zone finale, il doit avancer d'exactement une case
    if (cases[caseActuelle].type == "Final") {
        int finalStep = cases[caseActuelle].finalStep; // Étape actuelle en zone finale
        int nouvelleStep = finalStep + 1; // Étape cible
        if(valeurDe>=nouvelleStep){
            // ✅ Vérifier si on dépasse "FinalStep 6" = victoire
            if (nouvelleStep == 6) {
            std::string message = "🎉 " + playersInGame[joueurActuel].name + " a gagné la partie ! 🎉";
            texteActions.setString(message);
            std::cout << message << std::endl;
            isRunning = false; // Arrêter le jeu
            return true;
        }
            // ✅ Vérifier si la case suivante est disponible
            nouvelleCase = caseActuelle + 1;
            for (auto& pionsJoueur : playerPions) {
                for (auto& autrePion : pionsJoueur) {
                    if (&autrePion != &pion && autrePion.isOut && autrePion.sprite.getPosition() == cases[nouvelleCase].position) {
                        std::string message = "Joueur: " + playersInGame[joueurActuel].name + 
                                            "\nDeplac impossible!\nUn pion bloque \nla case finale.";
                        texteActions.setString(message);
                        return false; // Mouvement bloqué
                    }
                }
            }
        }else{
            return false;
        }
    }


    // ✅ Vérification des collisions avec un pion 
    for (auto& pionsJoueur : playerPions) {
        for (auto& autrePion : pionsJoueur) {
            if (&autrePion != &pion && autrePion.isOut && autrePion.sprite.getPosition() == cases[nouvelleCase].position) {
                std::string message;
                message = "Joueur: " + playersInGame[joueurActuel].name + 
                "\nDeplac impossible!\nUn pion bloque.";
                texteActions.setString(message);
                std::cout << message << std::endl;
                return false; // Mouvement bloqué
                }
            }
        }
   
    
    // Animation du deplacement du pion
    sf::Vector2f startPosition = pion.sprite.getPosition();
    sf::Vector2f endPosition = cases[nouvelleCase].position;
    sf::Clock clock;
    while (clock.getElapsedTime().asSeconds() < 1.0f) {
        float progress = clock.getElapsedTime().asSeconds();
        pion.sprite.setPosition(startPosition + progress * (endPosition - startPosition));
        render(window);
    }
    pion.sprite.setPosition(endPosition);

    // Jouer un son de plateau aleatoire
    int sonIndex = rand() % gameboardSounds.size();
    gameboardSounds[sonIndex].setVolume(500.f);
    gameboardSounds[sonIndex].play();

    std::string message = "Joueur: " + playersInGame[joueurActuel].name + "\nPion avance !";
    texteActions.setString(message);// Log the new position to the console
    std::cout << "Pion avance a la position: (" << cases[nouvelleCase].position.x << ", " << cases[nouvelleCase].position.y << ")" << std::endl; 

    message = "Joueur: " + playersInGame[joueurActuel].name + "\nPion avance !";
    texteActions.setString(message);
    std::cout << "Pion avance a la case: " << nouvelleCase << std::endl;

    return true;
}

// Passer au joueur suivant
void Jeu::passerAuJoueurSuivant() {
    if (!playersInGame.empty()) {
        if(valeurDe==6){
            valeurDe=0;
            diceRolled=false;
            attenteValidation = true;
            std::string message = "Joueur: " + playersInGame[joueurActuel].name + "\n=> ReLancez le de";
        texteActions.setString(message);
        std::cout << message << std::endl;
        return;
        }
        joueurActuel = (joueurActuel + 1) % playersInGame.size();
        valeurDe = 0; // Reinitialiser le de
        diceRolled=false; // Reinitialiser le de lance
        std::string message = "Joueur: " + playersInGame[joueurActuel].name + "\n=> Lancez le de";
        texteActions.setString(message);
        std::cout << message << std::endl;
        // 🔄 Mettre a jour l'apparence du pion actuel
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
            cases.push_back({yellowStart + sf::Vector2f(7 * delta, -6 * delta), "AvantFinal", 2});
        }
    }
    for (i = 0; i < 14; ++i){
        // 🔵 Cases Bleues +1 rouge
        if (i < 7) {
            cases.push_back({blueStart + sf::Vector2f(0, i * delta), "Parcours", 3});
        } else if (i < 13) {
            cases.push_back({blueStart + sf::Vector2f((i - 6) * delta, 6 * delta), "Parcours", 3});
        } else {
            cases.push_back({blueStart + sf::Vector2f(6 * delta, 7 * delta), "AvantFinal", 1});
        }
    }
    for (i = 0; i < 14; ++i){

        // 🔴 Cases Rouges +1 vert
        if (i < 7) {
            cases.push_back({redStart + sf::Vector2f(-i * delta, 0), "Parcours", 1});
        } else if (i < 13) {
            cases.push_back({redStart + sf::Vector2f(-6 * delta, (i - 6) * delta), "Parcours", 1});
        } else {
            cases.push_back({redStart + sf::Vector2f(-7 * delta, 6 * delta), "AvantFinal", 2});
        }
    }
    for (i = 0; i < 14; ++i){

        // 🟢 Cases Vertes +1 jaune
        if (i < 7) {
            cases.push_back({greenStart + sf::Vector2f(0, -i * delta), "Parcours", 2});
        } else if (i < 13) {
            cases.push_back({greenStart + sf::Vector2f(-(i - 6) * delta,- 6 * delta), "Parcours", 2});
        } else {
            cases.push_back({greenStart + sf::Vector2f(-6 * delta, -7 * delta), "AvantFinal", 0});
        }
    }
    // Ajouter les 6 cases "Finale" avant "Victoire" 
    for (i = 0; i < 6; ++i){
        cases.push_back({greenStart + sf::Vector2f(-6 * delta, -7 * delta)+ sf::Vector2f((i+1)*delta,0), "Final", 0,i+1}); //yellow final
    } 
    for (i = 0; i < 6; ++i){
        cases.push_back({yellowStart + sf::Vector2f(7 * delta, -6 * delta)+ sf::Vector2f(0,(i+1)*delta), "Final", 3,i+1}); //blue final
    } 
    for (i = 0; i < 6; ++i){
        cases.push_back({blueStart + sf::Vector2f(6 * delta, 7 * delta)+ sf::Vector2f(-(i+1)*delta,0), "Final", 1,i+1}); //red final
    } 
     for (i = 0; i < 6; ++i){
        cases.push_back({redStart + sf::Vector2f(-7 * delta, 6 * delta)+ sf::Vector2f(0,-(i+1)*delta), "Final", 2,i+1});//green final
    }  
    
    // // Ajouter la case "Victoire" au centre
    // for (int j=0;j<3;++j){
    // cases.emplace_back(sf::Vector2f(300.f, 300.f), "Victoire",j);
    // }
}

// Lancer la boucle du jeu
void Jeu::run(sf::RenderWindow& window) {
    isRunning = true;

    while (window.isOpen()) {
        handleEvents(window);
        update();
        render(window);

        // Si le jeu est terminé, afficher un écran de fin et attendre que le joueur ferme la fenêtre
        if (!isRunning) {
            sf::Text texteFin;
            texteFin.setFont(font);
            texteFin.setString("🎉 " + playersInGame[joueurActuel].name + " a gagné ! 🎉\nCliquez pour quitter.");
            texteFin.setCharacterSize(30);
            texteFin.setFillColor(sf::Color::Black);
            texteFin.setPosition(100.f, 300.f);

            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed || event.type == sf::Event::MouseButtonPressed) {
                        window.close(); // Fermer proprement
                    }
                }

                window.clear(sf::Color::White);
                window.draw(texteFin);
                window.display();
            }
        }
    }
}


// Mettre a jour la logique du jeu (placeholder pour futur ajout)
void Jeu::update() {
    // Ici, ajoutez les regles du jeu ou la logique de deplacement
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
