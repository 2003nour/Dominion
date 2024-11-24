#ifndef CARTES_HPP
#define CARTES_HPP

#include <string>

class Cartes {
public:
    // Constructeur
    Cartes(std::string nom);

    // Accesseurs pour les attributs (optionnel si vous en avez besoin ailleurs dans votre code)
    std::string getNom() const;
    std::string getType() const;
    int getCout() const;
    int getStock() const;
    std::string getDescription() const;

private:
    // Attributs de la classe Cartes
    std::string nom;
    std::string type;
    int cout;
    int stock;
    std::string description;
};

#endif // CARTES_HPP