#include <iostream>
#include <vector>
#include "Partie.h"
#include "Joueur.h"
#include "Cartes.hpp"

#include <iostream>
#include <vector>
#include "Partie.h"
int main() {
    std::cout << "Bienvenue dans le jeu Dominion !" << std::endl;

    // Initialisation des joueurs
    int nombreJoueurs;
    std::string saisie;
    std::cout << "Entrez le nombre de joueurs (2-4)  : ";
    std::cin >> saisie;

    if (saisie == "fin") {
        std::cout << "Jeu termine. A bientot !" << std::endl;
        return 0;
    }

    try {
        nombreJoueurs = std::stoi(saisie); 
    } catch (...) {
        std::cout << "Entrée invalide. Le jeu necessite un nombre de joueurs valide entre 2 et 4." << std::endl;
        return 1;
    }

    if (nombreJoueurs < 2 || nombreJoueurs > 4) {
        std::cout << "Nombre de joueurs invalide. Le jeu necessite entre 2 et 4 joueurs." << std::endl;
        return 1;
    }

    std::vector<std::string> nomsJoueurs;
    for (int i = 0; i < nombreJoueurs; ++i) {
        std::string nom;
        std::cout << "Entrez le nom du joueur " << i + 1 << "  : ";
        std::cin >> nom;

        if (nom == "fin") {
            std::cout << "Jeu termine. A bientot !" << std::endl;
            return 0;
        }

        nomsJoueurs.push_back(nom);
    }

    Partie partie(nomsJoueurs);
    std::cout << "\nLa partie commence !" << std::endl;

    // Lancement du jeu
    partie.demarrer();

    std::cout << "Merci d'avoir joue a Dominion ! a bientot !" << std::endl;
    return 0;
}