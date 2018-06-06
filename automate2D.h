#ifndef AUTOMATE2D_H_INCLUDED
#define AUTOMATE2D_H_INCLUDED

#include "autocell.h"

class Automate2D : public Automate{

    unsigned int nbMinVivant; /*sachant une cellule vivante, cb il faut de cellule vivante minimum pour qu'elle vive à t+1*/
    unsigned int nbMaxVivant;
    unsigned int nbMinMort; /*même choix pour une cellule morte*/
    unsigned int nbMaxMort;

public :
    Automate2D(unsigned int minV=0, unsigned int maxV=0, unsigned int minM=0, unsigned int maxM=0, unsigned int n=0);
    const unsigned int getMinV()const{return nbMinVivant;}
    const unsigned int getMaxV()const{return nbMaxVivant;}
    const unsigned int getMinM()const{return nbMinMort;}
    const unsigned int getMaxM()const{return nbMaxMort;}
    ~Automate2D()=default;
    void AppliquerTransition(const Etat& dep, Etat& dest) const;
    unsigned int CountVoisin(unsigned int li, unsigned int co, const Etat& e) const;
};

std::ostream& operator<<(std::ostream& f, const Automate2D& A);


#endif // AUTOMATE2D_H_INCLUDED
