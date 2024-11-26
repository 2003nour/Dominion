
#ifndef PARTIE_H
#define PARTIE_H

#include <vector>
#include "Joueur.h"
#include "Cartes.hpp"
#include <memory>

class Partie {
public:
    Partie(const std::vector<std::string>& nomsJoueurs);
    void demarrer();
    void jouerTour();
    bool estFinie() const;
    void annoncerVainqueur() const;
    void phaseAction(Joueur& joueur);
    void phaseAchat(Joueur& joueur);
    void phaseAjustement(Joueur& joueur);
    std::vector<Joueur>& getJoueurs() ;
    void jeterCarte(std::shared_ptr<Cartes> carte);
    void afficherRebuts() const ;
    std::vector<std::shared_ptr<Cartes>> reserve;
private:
    std::vector<Joueur> joueurs;
    
    std::vector<std::shared_ptr<Cartes>> rebuts;

    int joueurActuel;

    // Méthodes internes
    void initialiserReserve();
    void afficherEtat() const;

    // Nouvelles méthodes ajoutées
    void acheterCarte(Joueur& joueur);
    void activerModeFinDePartie();

};

#endif 