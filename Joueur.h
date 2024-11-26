#ifndef JOUEUR_H
#define JOUEUR_H

#include <vector>
#include <string>
#include "Cartes.hpp"
#include <memory>

class Joueur {
protected:
    int argentVirtuel;  // implémenté pour la carte action bucheron et avoir la possibilité d'avoir des piece en plus sans modifier la main
    std::string nom; 
    int pointsVictoire;
    int nombreActions;
    int nombreAchats;
    int argent;
    std::vector<std::shared_ptr<Cartes>> deck;
    std::vector<std::shared_ptr<Cartes>> main;
    std::vector<std::shared_ptr<Cartes>> defausse;

public:
    Joueur(const std::string& nom);
    void ajouterArgentVirtuel(int montant);
    void resetArgentVirtuel();
    int getArgentVirtuel() const;

    std::vector<std::shared_ptr<Cartes>>& getMain();
    void setMain(const std::vector<std::shared_ptr<Cartes>>& mano);
    int getArgent() const;
    void setArgent(int n_argent);
    void setDeck(const std::vector<std::shared_ptr<Cartes>>& nouveauDeck);
    void setNombreActions(int nbactions);
    void setNombreAchats(int nombreAchats);
    std::shared_ptr<Cartes> piocher(int nombre);
    void piocherMalediction();
    int calculerOrEnMain();
    void supprimerCarte(const std::shared_ptr<Cartes>& carte);
    int getNombreAchats() const;
    int getNombreActions() const;
    void ajouterArgent(int montant);
    int calculerPointsVictoire() const;
    void jouerAction(int indiceCarte);
    void acheterCarte(Cartes& carte);
    void finDuTour();
    bool possedeCarte(const std::string& nomCarte) const;
    void setPointsVictoire(int points);
    std::vector<std::shared_ptr<Cartes>>& getDeck();
    const std::string& getNom() const;
    int getPointsVictoire() const;
    void melangerDeck();
    std::vector<std::shared_ptr<Cartes>>& getDefausse();
    void afficherEtat() const;
    void verifierMain();
};


#endif 