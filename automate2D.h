#ifndef AUTOMATE2D_H_INCLUDED
#define AUTOMATE2D_H_INCLUDED

#include "autocell.h"

class Etat2D{ //grille de l'application

    unsigned int nbLigne;
    unsigned int nbColonne; //attributs valeurs et nbColonne dans la classe mère
    bool** valeurs; //tableau d'int plutôt

public:
    Etat2D() : nbLigne(0), nbColonne(0), valeurs(nullptr) {}
    Etat2D(unsigned int nbL, unsigned int nbC);
    const unsigned int getCol()const {return nbColonne;}
    const unsigned int getRow()const {return nbLigne;}
    bool getCellule(unsigned int i, unsigned int j) const;
    void setCellule(unsigned int i, unsigned int j, bool val);
    ~Etat2D();
    Etat2D(const Etat2D& e);
    Etat2D& operator=(const Etat2D& e);
    unsigned int CountVoisin(unsigned int li, unsigned int co) const;
};

std::ostream& operator<<(std::ostream& f, const Etat2D& e);

class Automate2D{

    unsigned int nbMinVivant; /*sachant une cellule vivante, cb il
    faut de cellule vivante minimum pour qu'elle vive à t+1*/
    unsigned int nbMaxVivant;
    unsigned int nbMinMort; /*même choix pour une cellule morte*/
    unsigned int nbMaxMort;

public :
    Automate2D(unsigned int minV=0, unsigned int maxV=0, unsigned int minM=0, unsigned int maxM=0);
    const unsigned int getMinV()const{return nbMinVivant;}
    const unsigned int getMaxV()const{return nbMaxVivant;}
    const unsigned int getMinM()const{return nbMinMort;}
    const unsigned int getMaxM()const{return nbMaxMort;}
    ~Automate2D()=default;
    void AppliquerTransition(const Etat2D& dep, Etat2D& dest) const;
};

class Simulateur2D{

    const Automate2D& automate;
    const Etat2D* depart=nullptr;
    Etat2D** etats=nullptr;
    unsigned int nbMaxEtats; //nb max qu'on sauvegarde dans le tableau etats
    unsigned int rang=0;

public :
    void setEtatDepart(const Etat2D& e);
    Simulateur2D(const Automate2D& a, unsigned int buf=2);
    Simulateur2D(const Automate2D& a, const Etat2D& dep, unsigned int buffer=2);
    void next();
    void run(unsigned int nbSteps);
    void reset();
    const Etat2D& dernier() const;
    unsigned int getRangDernier()const;
    ~Simulateur2D();

};

#endif // AUTOMATE2D_H_INCLUDED
