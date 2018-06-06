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


class Etat{

    unsigned int nbColonne;
    int* valeurs;
    //est-ce qu'on met un attribut dimension?

public:
    Etat(unsigned int n): nbColonne(n){}
    unsigned int getLargeur()const {return nbColonne;}
    virtual ~Etat(){}
    virtual unsigned int getHauteur() const = 0; //définis plus tard
    //fonction pour setLargeur ? pour ajuster l'état
};

class Automate{

    unsigned int nbEtat;

public:
    Automate(unsigned int n=1) :  nbEtat(n){}
    void setNbEtat(unsigned int m){nbEtat=e;}
    unsigned short int getNbEtat() const {return nbEtat;}
    virtual void appliquerTransition(const Etat&, Etat&) const =0; //methode virtuelle pure

};

class Simulateur{

    const Automate& automate;
    const Etat& depart=nullptr;
    Etat** etats=nullptr;
    unsigned int nbMaxEtats; //nb max qu'on sauvegarde dans le tableau etats
    unsigned int rang=0;

public :
    Simulateur(const Automate& a, unsigned int buf=2);
    Simulateur(const Automate& a, const Etat& dep, unsigned int buffer=2);
    void setEtatDepart(const Etat& e);
    void next();
    void run(unsigned int nbSteps);
    void reset();
    const Etat& dernier() const;
    unsigned int getRangDernier()const{return rang;}
    ~Simulateur();

};


#endif // AUTOCELL_H_INCLUDED
