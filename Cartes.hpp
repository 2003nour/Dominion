#ifndef CARTES_HPP
#define CARTES_HPP

#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Cartes {
protected:
    std::string nom;
    std::string type;
    std::string description;
    int cout;
    int stock;

public:
    static std::vector<std::shared_ptr<Cartes>> toutesLesCartes;

    // Constructeurs
    explicit Cartes(const std::string& nom);
    Cartes() = default;

    // Destructeur
    virtual ~Cartes();

    // Méthodes
    virtual std::string getType() const;
    std::string getNom() const;
    int getStock() const;
    std::string getDescription() const;
    int getCout() const;
    void setStock(int nouveauStock);

    bool operator==(const Cartes& autre) const;
};

#endif // CARTES_HPP