#ifndef AUTOCELL_H_INCLUDED
#define AUTOCELL_H_INCLUDED

#include <string>
#include <iostream>
#include<typeinfo>


using namespace std;

class AutomateException {
public:
AutomateException(const std::string& message):info(message) {}
std::string getInfo() const { return info; }
private:
std::string info;
};

short unsigned int NumBitToNum(const std::string& num);
std::string NumToNumBit(short unsigned int num);

class Etat{

    unsigned int nbColonne;
    unsigned int nbLigne; // =1 quand 1D
    bool** valeurs;

public:
    Etat(unsigned int c, unsigned int l);
    Etat() : nbLigne(0), nbColonne(0), valeurs(nullptr) {}
    unsigned int getLargeur()const {return nbColonne;}
    unsigned int getHauteur()const {return nbLigne;}
    //utile de faire une fonction quelle_dim ?
    ~Etat();
    bool getCellule(unsigned int i, unsigned int j) const;
    void setCellule(unsigned int i, unsigned int j, bool val);
	Etat(const Etat& e);
	Etat& operator=(const Etat& e);

};

std::ostream& operator<<(std::ostream& f, const Etat& e);


class Automate{

    unsigned int nbEtat; //pour l'instant seulement 2

public:
    Automate(unsigned int n=1) :  nbEtat(n){}
    void setNbEtat(unsigned int m){nbEtat=m;}
    unsigned short int getNbEtat() const {return nbEtat;}
    virtual void AppliquerTransition(const Etat&, Etat&) const =0; //methode virtuelle pure
    ~Automate()=default;
};


class Simulateur{

    const Automate& automate;
    const Etat* depart;
    Etat** etats;
    unsigned int nbMaxEtats=0; //nb max qu'on sauvegarde dans le tableau etats
    unsigned int rang=0;
    void build(unsigned int c);
    Simulateur(const Simulateur& s)=delete;
    Simulateur& operator=(const Simulateur& s)=delete;
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
