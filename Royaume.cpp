
#include "Royaume.hpp"
#include "Joueur.h"
#include "Partie.h"
#include "Cartes.hpp"



#include <iostream>   
#include <set>
#include <algorithm> // Nécessaire pour std::find_if

// Constructeur
Royaume::Royaume(const std::string& nom) : Cartes(nom) {
    type = "Royaume";
    stock = 10;

    if (nom == "Village") {
        cout = 3;
        description = "Permet de piocher 1 carte et de gagner 2 actions supplémentaires.";
    } else if (nom == "Laboratoire") {
        cout = 5;
        description = "Permet de piocher 2 cartes et de gagner 1 action supplémentaire.";
    } else if (nom == "Chapelle") {
        cout = 2;
        description = "Permet de défausser jusqu'à 4 cartes.";
    } else if (nom == "Sorciere") {
        cout = 5;
        description = "Permet de piocher 2 cartes et de distribuer une malédiction aux adversaires.";
    } else if (nom == "Atelier") {
        cout = 3;
        description = "Permet de gagner une carte coûtant jusqu'à 4.";
    } else if (nom == "Voleur") {
        cout = 4;
        description = "Les adversaires révèlent les 2 premières cartes de leur deck, et vous en piquez une si elle est de type trésor.";
    } else if (nom == "Bucheron") {
        cout = 3;
        description = "Permet de gagner 2 pièces virtuelles et un achat supplémentaire.";
    } else if (nom == "Douve") {
        cout = 2;
        description = "Permet de se protéger des attaques adverses.";
    } else if (nom == "Festin") {
        cout = 4;
        description = "Permet de gagner une carte coûtant jusqu'à 5, jetée après utilisation.";
    } else if (nom == "Jardins") {
        cout = 4;
        description = "Vous rapporte 1 point de victoire pour chaque multiple de 10 cartes dans votre deck.";
    } else {
        std::cout << "Erreur : Carte Royaume inconnue." << std::endl;
    }

    toutesLesCartes.push_back(std::make_shared<Royaume>(*this));
}

// Destructeur
Royaume::~Royaume() {}

// Atelier testé
void Royaume::Atelier(Joueur& joueur, Partie& partie) {
    std::cout << "Quelle carte souhaitez-vous ajouter à votre deck (coût <= 4) ?\n";

    // Filtrer les cartes valides (coût <= 4 et en stock), tout en évitant les doublons
    std::vector<std::shared_ptr<Cartes>> cartesDisponibles;
    std::set<std::string> nomsCartesAjoutes; // Pour éviter les doublons

    for (const auto& carte : partie.reserve) { // Utiliser les cartes directement depuis la réserve
        if (carte->getCout() <= 4 && carte->getStock() > 0 && nomsCartesAjoutes.find(carte->getNom()) == nomsCartesAjoutes.end()) {
            cartesDisponibles.push_back(carte);
            nomsCartesAjoutes.insert(carte->getNom());
        }
    }

    // Vérifiez qu'il y a au moins une carte disponible
    if (cartesDisponibles.empty()) {
        std::cout << "Aucune carte disponible avec un coût <= 4.\n";
        return;
    }

    // Afficher les cartes valides
    for (size_t i = 0; i < cartesDisponibles.size(); ++i) {
        std::cout << i + 1 << " - " << cartesDisponibles[i]->getNom()
                << " (Coût : " << cartesDisponibles[i]->getCout()
                << ", Stock : " << cartesDisponibles[i]->getStock() << ")\n";
    }

    // Choix du joueur
    int choix;
    std::cout << "Votre choix : ";
    std::cin >> choix;

    // Vérification du choix
    if (choix > 0 && static_cast<size_t>(choix) <= cartesDisponibles.size()) {
        auto carteChoisie = cartesDisponibles[choix - 1];

        // DEBUG : Afficher le stock avant décrémentation
        std::cout << "DEBUG: Stock avant décrémentation pour " << carteChoisie->getNom()
                << ": " << carteChoisie->getStock() << "\n";

        carteChoisie->setStock(carteChoisie->getStock() - 1);

        joueur.getDefausse().push_back(carteChoisie);
        std::cout << "La carte " << carteChoisie->getNom() << " a été ajoutée à votre défausse.\n";
    } else {
        std::cout << "Choix invalide. Aucune carte ajoutée.\n";
    }
}



// Bucheron testé
void Royaume::Bucheron(Joueur& joueur) {
    std::cout << "Effet Bûcheron en cours...\n";

    // Ajouter 2 cartes Cuivre directement dans la main
    for (int i = 0; i < 2; ++i) {
        auto carteCuivre = std::make_shared<Cartes>("Cuivre");
        joueur.getMain().push_back(carteCuivre); // Ajoute les cuivres à la main
    }

    // Augmenter le nombre d'achats
    joueur.setNombreAchats(joueur.getNombreAchats() + 1);



    // Afficher le résultat de l'effet
    std::cout << joueur.getNom() << " gagne 2 pièces (Cuivres ajoutés à la main) et un achat supplémentaire.\n";

    // Afficher la main du joueur après l'effet
    std::cout << "Main actuelle de " << joueur.getNom() << " :\n";
    for (const auto& carte : joueur.getMain()) {
        std::cout << "- " << carte->getNom() << " (" << carte->getType() << ")\n";
    }
}




void Royaume::Chapelle(Joueur& joueur) {
    int choix;
    int cartesADefauser = 0;

    std::cout << "Vous pouvez supprimer définitivement jusqu'à 4 cartes de votre main.\n";

    while (cartesADefauser < 4 && !joueur.getMain().empty()) {
        // Afficher les cartes disponibles dans la main
        std::cout << "Main actuelle :\n";
        for (size_t i = 0; i < joueur.getMain().size(); ++i) {
            std::cout << i + 1 << " - " << joueur.getMain()[i]->getNom() 
                    << " (" << joueur.getMain()[i]->getDescription() << ")\n";
        }
        std::cout << "Choisissez une carte à supprimer (0 pour terminer) : ";
        std::cin >> choix;

        // Si le joueur entre "0", il arrête la suppression
        if (choix == 0) {
            break;
        }

        // Vérification que le choix est valide
        if (choix > 0 && static_cast<size_t>(choix) <= joueur.getMain().size()) {
            // Supprimer définitivement la carte choisie
            auto carteSupprimee = joueur.getMain()[choix - 1];
            joueur.getMain().erase(joueur.getMain().begin() + choix - 1);
            cartesADefauser++;

            // Affichage pour le joueur
            std::cout << "La carte " << carteSupprimee->getNom() 
                    << " a été retirée définitivement du jeu.\n";

        } else {
            std::cout << "Choix invalide. Veuillez réessayer.\n";
        }
    }

    std::cout << "Vous avez supprimé " << cartesADefauser 
            << " carte(s) de votre main.\n";
}


// Douve pas besoin de test
void Royaume::Douve(Joueur& joueur) {
        joueur.piocher(2);
        std::cout << joueur.getNom() << " pioche 2 cartes supplémentaires.\n";
    }


// Festin testé
void Royaume::Festin(Joueur& joueur, Partie& partie) {
    std::cout << "Quelle carte souhaitez-vous gagner (coût <= 5) ?\n";

    
    std::vector<std::shared_ptr<Cartes>> cartesDisponibles;
    std::set<std::string> nomsCartesAjoutes; // Pour éviter les doublons

    for (const auto& carte : partie.reserve) { // Utiliser les cartes directement depuis la réserve
        if (carte->getCout() <= 5 && carte->getStock() > 0 && nomsCartesAjoutes.find(carte->getNom()) == nomsCartesAjoutes.end()) { // Filtrer les cartes valides (coût <= 5 et en stock)  tout en éliminant les doublons
            cartesDisponibles.push_back(carte);
            nomsCartesAjoutes.insert(carte->getNom());
        }
    }

    // Vérifiez qu'il y a des cartes disponibles
    if (cartesDisponibles.empty()) {
        std::cout << "Aucune carte disponible avec un coût <= 5.\n";
        return;
    }

    // Afficher les cartes valides
    for (size_t i = 0; i < cartesDisponibles.size(); ++i) {
        std::cout << i + 1 << " - " << cartesDisponibles[i]->getNom()
                << " (Coût : " << cartesDisponibles[i]->getCout()
                << ", Stock : " << cartesDisponibles[i]->getStock() << ")\n";
    }

    // Choix du joueur
    int choix;
    std::cout << "Votre choix : ";
    std::cin >> choix;

    if (choix > 0 && static_cast<size_t>(choix) <= cartesDisponibles.size()) {
        auto carteChoisie = cartesDisponibles[choix - 1];

        carteChoisie->setStock(carteChoisie->getStock() - 1);

        joueur.getDefausse().push_back(carteChoisie);
        std::cout << "Vous avez choisi : " << carteChoisie->getNom() << ". Elle a été ajoutée à votre défausse.\n";

        // Jeter la carte Festin dans les rebuts
        partie.jeterCarte(std::make_shared<Royaume>("Festin"));
    } else {
        std::cout << "Choix invalide. Aucune carte gagnée.\n";
    }
}


// Jardins
void Royaume::Jardins(Joueur& joueur,Partie& partie) {
    if (this->nom == "Jardins" && partie.estFinie()) {
        joueur.setPointsVictoire(joueur.getPointsVictoire() + joueur.getDeck().size() / 10);
    }
}

// Laboratoire testé
void Royaume::Laboratoire(Joueur& joueur) {
        joueur.piocher(2);
        joueur.setNombreActions(joueur.getNombreActions() + 1);
        std::cout << joueur.getNom() << " pioche 2 cartes et gagne une action supplémentaire.\n";
    }


// testés
void Royaume::Village(Joueur& joueur) {
    std::cout << joueur.getNom() << " a pioche 1 carte et gagne 2 actions supplémentaires.\n";

    // Appeler la méthode pour piocher une carte
    auto cartePiochee = joueur.piocher(1);

    // DEBUG : Vérification que la carte est bien piochée
    if (cartePiochee) {
        std::cout << "DEBUG: Carte piochée : " << cartePiochee->getNom() << "\n";
    } else {
        std::cout << "DEBUG: Aucune carte piochée, deck ou défausse vide.\n";
    }

    // Ajouter 2 actions supplémentaires
    joueur.setNombreActions(joueur.getNombreActions() + 2);

    // DEBUG : Afficher les actions restantes
    std::cout << "DEBUG: Nombre d'actions restantes après Village : " << joueur.getNombreActions() << "\n";

    // DEBUG : Afficher la main après avoir pioché
    std::cout << "Main après avoir joué Village :\n";
    for (const auto& carte : joueur.getMain()) {
        std::cout << "- " << carte->getNom() << " (" << carte->getDescription() << ")\n";
    }
}

// testé
void Royaume::Sorciere(Joueur& joueur, Partie& partie) {
        joueur.piocher(2);
        for (auto& autreJoueur : partie.getJoueurs()) {
            if (&autreJoueur != &joueur && !autreJoueur.possedeCarte("Douve")) {
                autreJoueur.piocherMalediction();
            }
            else{  std::cout << autreJoueur.getNom() << " a une Douve.\n"; }
        }
    }


//  testé
void Royaume::Voleur(Joueur& joueur, Partie& partie) {
    for (Joueur& autreJoueur : partie.getJoueurs()) { // Référence non-constante pour permettre modification
        if (&autreJoueur != &joueur && !autreJoueur.possedeCarte("Douve")) { // Ne pas cibler le joueur lui-même
            std::cout << autreJoueur.getNom() << " n'a pas de Douve. Les deux premières cartes de son deck seront révélées.\n";

            // Vérifier que le deck contient au moins deux cartes
            auto& deck = autreJoueur.getDeck();
            size_t cartesARemontrer = std::min(deck.size(), static_cast<size_t>(2));

            if (cartesARemontrer == 0) {
                std::cout << autreJoueur.getNom() << " n'a pas assez de cartes dans son deck.\n";
                continue; // Passer au prochain joueur
            }

            for (size_t i = 0; i < cartesARemontrer; ++i) {
                auto carte = deck[i]; // Récupérer la carte
                std::cout << autreJoueur.getNom() << " révèle : " << carte->getNom() 
                        << " (" << carte->getType() << ")\n";

                // Vérifier si la carte est un Trésor
                if (carte->getType() == "Tresor") {
                    std::cout << "Souhaitez-vous voler " << carte->getNom() << " ? (oui/non): ";
                    std::string reponse;
                    std::cin >> reponse;

                    if (reponse == "oui") {
                        // Déplacer la carte dans la défausse du joueur
                        joueur.getDefausse().push_back(carte);

                        // Supprimer la carte du deck de l'autre joueur
                        deck.erase(deck.begin() + i);
                        std::cout << "Vous avez vole " << carte->getNom() << ". Elle a ete ajoutee à votre defausse.\n";

                        // Réduire l'indice pour tenir compte du décalage dans le deck après l'effacement
                        --cartesARemontrer;
                        --i;
                    } else {
                        std::cout << "Vous avez choisi de ne pas voler " << carte->getNom() << ".\n";
                    }
                }
            }
        } else {
            std::cout << autreJoueur.getNom() << " est protege par une Douve.\n";
        }
    }

    // Jeter la carte Voleur après son utilisation
    partie.jeterCarte(std::make_shared<Royaume>("Voleur"));
}
