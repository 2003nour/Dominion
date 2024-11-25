#ifndef ROYAUME_HPP
#define ROYAUME_HPP

#include "Cartes.hpp"
#include "Joueur.h"
#include "Partie.h"
#include <string>
#include <memory>



class Royaume : public Cartes {
    private :
    std::shared_ptr<Cartes> carteDeBase;

    std::string nom;
public:
    // Constructeur et destructeur
    Royaume(const std::string& nom);

    ~Royaume();

    // Effets des cartes Royaume
    void Atelier(Joueur& joueur, Partie& partie);
    void Bucheron(Joueur& joueur);
    void Chapelle(Joueur& joueur);
    void Laboratoire(Joueur& joueur);
    void Village(Joueur& joueur);
    void Douve(Joueur& joueur);
    void Festin(Joueur& joueur, Partie& partie);
    void Jardins(Joueur& joueur,Partie& partie);
    void Sorciere(Joueur& joueur, Partie& partie);
    void Voleur(Joueur& joueur, Partie& partie);
    

};

#endif 