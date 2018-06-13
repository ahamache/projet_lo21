#ifndef ETAT_H_INCLUDED
#define ETAT_H_INCLUDED

#include <string>
#include <iostream>
using namespace std;

class EtatException {
public:
    EtatException(const std::string& message):info(message) {}
    std::string getInfo() const { return info; }
private:
    std::string info;
};

class Etat{

    unsigned int nbColonne;
    unsigned int nbLigne; // =1 quand 1D
    int** valeurs;

public:
    Etat(unsigned int c, unsigned int l);
    Etat() : nbColonne(0), nbLigne(0), valeurs(nullptr) {}
    unsigned int getLargeur()const {return nbColonne;}
    unsigned int getLongueur()const {return nbLigne;}
    ~Etat();
    int getCellule(unsigned int i, unsigned int j) const;
    void setCellule(unsigned int i, unsigned int j, int val); //on passe de bool à int pour pouvoir mieux s'adapter à d'autres automates
    Etat(const Etat& e);
    Etat& operator=(const Etat& e);

};

std::ostream& operator<<(std::ostream& f, const Etat& e);

#endif // ETAT_H_INCLUDED
