#include "Cartes.hpp"
#include <iostream>
#include "Cartes.hpp"

std::vector<std::shared_ptr<Cartes>> Cartes::toutesLesCartes;

//constructeur
Cartes::Cartes(const std::string& nom) : nom(nom), cout(0), stock(0) {
    if (nom == "Or" || nom == "Argent" || nom == "Cuivre") {
        type = "Tresor";
        if (nom == "Or") {
            cout = 6;
            stock = 30;
            description = "Permet de gagner 3 pièces.";
        } else if (nom == "Argent") {
            cout = 3;
            stock = 40;
            description = "Permet de gagner 2 pièces.";
        } else if (nom == "Cuivre") {
            cout = 0;
            stock = 60;
            description = "Permet de gagner 1 pièce.";
        }
    } else if (nom == "Domaine" || nom == "Duché" || nom == "Province" || nom == "Malediction") {
        type = "Victoire";
        if (nom == "Domaine") {
            cout = 2;
            stock = 24;
            description = "Vous rapporte 1 point de victoire.";
        } else if (nom == "Duché") {
            cout = 5;
            stock = 12;
            description = "Vous rapporte 3 points de victoire.";
        } else if (nom == "Province") {
            cout = 8;
            stock = 8; // Peut varier selon le nombre de joueurs
            description = "Vous rapporte 6 points de victoire.";
        } else if (nom == "Malediction") {
            cout = 0;
            stock = 30;
            description = "Vous retire 1 point de victoire.";
        }
    }
    toutesLesCartes.push_back(std::make_shared<Cartes>(*this));
}

std::string Cartes::getType() const {
    return type;
}
std::string Cartes::getNom() const {
    return nom;
}
int Cartes::getStock() const {
    return stock;
}
std::string Cartes::getDescription() const {
    return description;
}
int Cartes::getCout() const {
    return cout;
}
void Cartes::setStock(int nouveauStock) {
    stock = nouveauStock;
}

//destructeur
Cartes::~Cartes() {
}

bool Cartes::operator==(const Cartes& autre) const {
    return this->nom == autre.nom;
}