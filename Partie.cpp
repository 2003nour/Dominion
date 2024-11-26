

#include "Partie.h"
#include "Joueur.h"
#include "Cartes.hpp"
#include "Royaume.hpp"
#include <iostream>
#include <limits> 

Partie::Partie(const std::vector<std::string>& nomsJoueurs) : joueurActuel(0) {
    for (const auto& nom : nomsJoueurs) {
        joueurs.emplace_back(nom);
    }
    initialiserReserve();
}

void Partie::demarrer() {
    while (!estFinie()) {
        jouerTour();
        joueurActuel = (joueurActuel + 1) % joueurs.size();
    }
    for (const auto& joueur : joueurs) {
        joueur.afficherEtat();}
    annoncerVainqueur();
}

void Partie::jouerTour() {
    Joueur& joueur = joueurs[joueurActuel];
    std::cout << "\nTour de " << joueur.getNom() << "\n";

    // Afficher l'état initial du joueur
    afficherEtat();

    // Phase d'Action
    phaseAction(joueur);

    // Phase d'Achat
    phaseAchat(joueur);

    // Phase d'Ajustement
    phaseAjustement(joueur);

    // Fin du tour
    std::cout << "Fin du tour de " << joueur.getNom() << ".\n";
}


void Partie::phaseAction(Joueur& joueur) {
    std::cout << "\nPhase d'Action pour " << joueur.getNom() << "\n";
    
    joueur.verifierMain();
    while (joueur.getNombreActions() > 0) {
        // Récupérer la référence à la main du joueur
        auto& Mano = joueur.getMain(); 
        // Afficher la main
        std::cout << "Cartes disponibles dans votre main :\n";
        for (size_t i = 0; i < Mano.size(); ++i) {
            std::cout << i + 1 << " - " << Mano[i]->getNom() << " (" << Mano[i]->getDescription() << ")\n";
        }

        int choix;
        std::cout << "Choisissez une carte Action  (0 pour passer) : ";
        std::cin >> choix;
        if (choix== -1) {
            activerModeFinDePartie();
            return; // Terminez immédiatement cette phase
        }
        if (choix == 0) {
            // Le joueur décide de ne pas jouer d'autres actions
            break;
        }

        if (choix > 0 && static_cast<size_t>(choix) <= Mano.size()) {
            auto carteChoisie = Mano[choix - 1];
            std::cout << "Carte choisie : " << carteChoisie->getNom() << "\n";
            std::cout << "Type de carte : " << carteChoisie->getType() << "\n";

            if (carteChoisie->getType() == "Royaume") {
                // Gestion des effets spécifiques aux cartes Action
                if (carteChoisie->getNom() == "Village") {
                    std::cout << "Effet : Village\n";
                    Royaume("Village").Village(joueur);
                } else if (carteChoisie->getNom() == "Laboratoire") {
                    std::cout << "Effet : Laboratoire\n";
                    Royaume("Laboratoire").Laboratoire(joueur);
                } else if (carteChoisie->getNom() == "Chapelle") {
                    std::cout << "Effet : Chapelle\n";
                    Royaume("Chapelle").Chapelle(joueur);
                } else if (carteChoisie->getNom() == "Sorciere") {
                    std::cout << "Effet : Sorcière\n";
                    Royaume("Sorciere").Sorciere(joueur, *this);
                    for (auto& carte : reserve) {
                        if (carte->getNom() == "Malediction" && carte->getStock() > 0) {
                            carte->setStock(carte->getStock() - 1);
                            break;// pour éviter de parcourir inutilement une fois qu'on a trouvé
                        }
                    }
                } else if (carteChoisie->getNom() == "Atelier") {
                    std::cout << "Effet : Atelier\n";
                    Royaume("Atelier").Atelier(joueur,*this);
                } else if (carteChoisie->getNom() == "Voleur") {
                    std::cout << "Effet : Voleur\n";
                    Royaume("Voleur").Voleur(joueur, *this);
                    joueur.supprimerCarte(carteChoisie);
                } else if (carteChoisie->getNom() == "Bucheron") {
                    std::cout << "Effet : Bucheron\n";
                    Royaume("Bucheron").Bucheron(joueur);
                } else if (carteChoisie->getNom() == "Douve") {
                    std::cout << "Effet : Douve\n";
                    Royaume("Douve").Douve(joueur);
                } else if (carteChoisie->getNom() == "Festin") {
                    std::cout << "Effet : Festin\n";
                    Royaume("Festin").Festin(joueur, *this);
                    joueur.supprimerCarte(carteChoisie);
                } else if (carteChoisie->getNom() == "Jardins") {
                    std::cout << "Effet : Jardins\n";
                    Royaume("Jardins").Jardins(joueur, *this);
                } else {
                    std::cout << "Effet inconnu pour la carte : " << carteChoisie->getNom() << "\n";
                }

                // Déplacer la carte dans la défausse après utilisation
                if (carteChoisie->getNom() != "Voleur" && carteChoisie->getNom() != "Festin") {
                
                joueur.getDefausse().push_back(carteChoisie);
                Mano.erase(Mano.begin() + choix - 1);}

                // Décrémenter le nombre d'actions
                joueur.setNombreActions(joueur.getNombreActions() - 1);
            } else {
                std::cout << "Erreur : La carte choisie n'est pas une carte Action.\n";
            }
        } else {
            std::cout << "Choix invalide. Aucune carte Action jouée.\n";
        }

        // Si le joueur n'a plus d'actions, afficher un message
        if (joueur.getNombreActions() <= 0) {
            std::cout << joueur.getNom() << " n'a plus d'actions disponibles.\n";
        }
    }
}
void Partie::phaseAchat(Joueur& joueur) {
    joueur.calculerOrEnMain(); // Met à jour l'argent disponible avant d'entrer dans la phase d'achat
    
    std::cout << "\nPhase d'Achat pour " << joueur.getNom() << "\n";

    while (joueur.getNombreAchats() > 0) {
        int argentDisponible = joueur.calculerOrEnMain();
        std::cout << "Or disponible dans la main : " << argentDisponible << " pièces\n";

        // Affichage des cartes dans la réserve
        for (size_t i = 0; i < reserve.size(); ++i) {
            auto carte = reserve[i];
            std::cout << i + 1 << " - " << carte->getNom() << " (Cout : " << carte->getCout()
                    << ", Stock : " << carte->getStock() << ")\n";
        }

        int choix;
        std::cout << "Choisissez une carte à acheter (0 pour passer) : ";
        std::cin >> choix;

        if (choix > 0 && static_cast<size_t>(choix) <= reserve.size()) {
            auto carte = reserve[choix - 1];
            if (argentDisponible >= carte->getCout() && carte->getStock() > 0) {
                joueur.acheterCarte(*carte);
            } else {
                std::cout << "Pas assez d'or ou stock épuisé.\n";
            }
        } 
        else if (choix == 0) {
            break;
        }
        if (choix== -1) {
            activerModeFinDePartie();
            return; // Terminez immédiatement cette phase
        }

        // Recalculer l'argent disponible après chaque achat
        joueur.calculerOrEnMain();
    }

    // Remettre l'argent virtuel à zéro après la phase d'achat
    joueur.resetArgentVirtuel();
    std::cout << "Argent virtuel réinitialisé après la phase d'achat.\n";
}



void Partie::phaseAjustement(Joueur& joueur) {
    std::cout << "\nPhase d'Ajustement pour " << joueur.getNom() << "\n";
    joueur.finDuTour();
    joueur.verifierMain();
    joueur.calculerOrEnMain();

    // Afficher les cartes jetées dans les rebuts
    afficherRebuts();

    std::cout << joueur.getNom() << " a terminé son tour.\n";
}
void Partie::jeterCarte(std::shared_ptr<Cartes> carte) {
    // Parcourir les joueurs pour retirer la carte de leurs collections
    for (auto& joueur : joueurs) {
        joueur.supprimerCarte(carte); // Appeler la méthode pour retirer la carte du joueur
    }

    // Ajouter la carte aux rebuts
    rebuts.push_back(carte);
    std::cout << "La carte " << carte->getNom() << " a ete jetee dans les rebuts.\n";
}


void Partie::afficherRebuts() const {
    std::cout << "Cartes dans les rebuts :\n";
    for (const auto& carte : rebuts) {
        std::cout << "- " << carte->getNom() << " (Type : " << carte->getType() << ")\n";
    }
}



void Partie::acheterCarte(Joueur& joueur) {
    std::cout << "Cartes disponibles dans la reserve :\n";
    for (size_t i = 0; i < reserve.size(); ++i) {
        std::cout << i + 1 << " - " << reserve[i]->getNom() << " (Cout : " << reserve[i]->getCout() 
                << ", Stock : " << reserve[i]->getStock() << ")\n";
    }

    int choixCarte;
    std::cout << "Choisissez une carte a acheter (0 pour annuler) : ";
    std::cin >> choixCarte;

    if (choixCarte > 0 && static_cast<size_t>(choixCarte) <= reserve.size()) {
        auto carte = reserve[choixCarte - 1];
        if (joueur.getArgent() >= carte->getCout() && carte->getStock() > 0) {
            joueur.acheterCarte(*carte);
            carte->setStock(carte->getStock() - 1);
            std::cout << joueur.getNom() << " a acheté " << carte->getNom() << ".\n";
        } else {
            std::cout << "Pas assez de pièces ou stock epuise.\n";
        }
    }
}

bool Partie::estFinie() const {
    // Condition de fin : la pile de cartes "Province" est vide
    for (const auto& carte : reserve) {
        if (carte->getNom() == "Province" && carte->getStock() == 0) {
            return true;
        }
    }

    // Condition de fin : au moins trois piles de la réserve sont vides
    int pilesVides = 0;
    for (const auto& carte : reserve) {
        if (carte->getStock() == 0) {
            pilesVides++;
        }
    }
    return pilesVides >= 3;
}

void Partie::annoncerVainqueur() const {
    std::cout << "La partie est terminee !\n";
    int maxPoints = 0;
    std::string nomVainqueur;

    for (const auto& joueur : joueurs) {
        int points = joueur.calculerPointsVictoire();
        std::cout << joueur.getNom() << " a " << points << " points de victoire.\n";
        if (points > maxPoints) {
            maxPoints = points;
            nomVainqueur = joueur.getNom();
        }
    }
    std::cout << "Le vainqueur est " << nomVainqueur << " avec " << maxPoints << " points de victoire !\n";
}
void Partie::activerModeFinDePartie() {
    // Réduire le stock de la carte "Province" à 1
    for (auto& carte : reserve) {
        if (carte->getNom() == "Province") {
            carte->setStock(1);
            std::cout << "Mode fin de partie activé : Le stock de la carte Province est désormais de 1.\n";
        }
    }

    // Remplacer les mains des joueurs par 5 cartes "Or"
    for (auto& joueur : joueurs) {
        joueur.getMain().clear(); // Vider la main actuelle
        for (int i = 0; i < 5; ++i) {
            auto carteOr = std::make_shared<Cartes>("Or");
            joueur.getMain().push_back(carteOr);
        }
 // Mettre à jour l'argent directement
        std::cout << "La main de " << joueur.getNom() << " a été remplie avec 5 cartes Or (valant 6 chacune).\n";
    }
}



void Partie::initialiserReserve() {
    reserve.push_back(std::make_shared<Cartes>("Or"));
    reserve.push_back(std::make_shared<Cartes>("Argent"));
    reserve.push_back(std::make_shared<Cartes>("Cuivre"));
    reserve.push_back(std::make_shared<Cartes>("Domaine"));
    reserve.push_back(std::make_shared<Cartes>("Duche"));
    reserve.push_back(std::make_shared<Cartes>("Province"));
    reserve.push_back(std::make_shared<Cartes>("Malediction"));

    // Cartes de type Royaume
    reserve.push_back(std::make_shared<Royaume>("Village"));
    reserve.push_back(std::make_shared<Royaume>("Laboratoire"));
    reserve.push_back(std::make_shared<Royaume>("Chapelle"));
    reserve.push_back(std::make_shared<Royaume>("Sorciere"));
    reserve.push_back(std::make_shared<Royaume>("Atelier"));
    reserve.push_back(std::make_shared<Royaume>("Voleur"));
    reserve.push_back(std::make_shared<Royaume>("Bucheron"));
    reserve.push_back(std::make_shared<Royaume>("Douve"));
    reserve.push_back(std::make_shared<Royaume>("Festin"));
    reserve.push_back(std::make_shared<Royaume>("Jardins"));
}


std::vector<Joueur>& Partie::getJoueurs() {
    return joueurs;
}
void Partie::afficherEtat() const {
    std::cout << "\nCartes dans la reserve :\n";
    for (const auto& carte : reserve) {
        std::cout << "- " << carte->getNom() << " (Stock : " << carte->getStock() << ", Cout : " << carte->getCout() << ")\n";
    }
} 