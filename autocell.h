#ifndef AUTOCELL_H_INCLUDED
#define AUTOCELL_H_INCLUDED

#include <string>
#include <iostream>

using namespace std;

class AutomateException {
public:
AutomateException(const std::string& message):info(message) {}
std::string getInfo() const { return info; }
private:
std::string info;
};

class Etat2D{ //grille de l'application

    unsigned int nbLigne=0;
    unsigned int nbColonne=0;
    bool** valeurs;

public:

    Etat2D(unsigned int nbL=0, unsigned int nbC=0);
    const unsigned int getCol()const {return nbColonne;}
    const unsigned int getRow()const {return nbLigne;}
    bool getCellule(unsigned int i, unsigned int j) const;
    void setCellule(unsigned int i, unsigned int j, bool val);
    ~Etat2D();
    void afficherGrille() const;
    Etat2D(const Etat2D& e);
    Etat2D& operator=(const Etat2D& e);
    unsigned int CountVoisin(unsigned int li, unsigned int co);
};

std::ostream& operator<<(std::ostream& f, const Etat2D& e);

class Automate2D{

    unsigned int nbMinVivant; /*sachant une cellule vivante, cb il
    faut de cellule vivante minimum pour qu'elle vive à t+1*/
    unsigned int nbMaxVivant;
    unsigned int nbMinMort; /*même choix pour une cellule morte*/
    unsigned int nbMaxMort;
    //--> vérifier que min<max
public :
    Automate2D(unsigned int minV=0, unsigned int maxV=0, unsigned int
               minM=0, unsigned int maxM=0) : nbMinVivant(minV),
               nbMaxVivant(maxV), nbMinMort(minM), nbMaxMort(maxM){}
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
    unsigned int nbMaxEtats=0; //nb max qu'on sauvegarde dans le tableau etats
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

#endif // AUTOCELL_H_INCLUDED
