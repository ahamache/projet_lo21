#include "automate2D.h"

unsigned int Automate2D::CountVoisin(unsigned int li, unsigned int co, const Etat& e)const{

    if(li<2) //on vérifie que l'automate est 2D car sinon pas les mêmes règles
        throw("L'automate n'est pas 2D");

    unsigned int nb=0;
    unsigned int limH, limB, limG, limD;

    if(li==0)
        limH=li;
    else limH=li-1;

    if(li==e.getHauteur()-1)
        limB=li;
    else limB=li+1;

    if(co==0)
        limG=co;
    else limG=co-1;

    if(co==e.getLargeur()-1)
        limD=co;
    else limD=co+1;

    for(unsigned int i=limH; i<=limB; i++){
        for(unsigned int j=limG; j<=limD; j++){
                if(e.getCellule(i,j))
                    nb+=1;
        }
    }
    if(e.getCellule(li, co))
        nb-=1;
    return nb;
}

Automate2D::Automate2D(unsigned int minV, unsigned int maxV, unsigned int minM, unsigned int maxM, unsigned int n): Automate(n){ //n est le nb d'état attribut de la classe mère

    if (minV>maxV || minM>maxV)
        throw AutomateException("minimum et maximum invalide");
    nbMinVivant=minV;
    nbMaxVivant=maxV;
    nbMinMort=minM;
    nbMaxMort=maxM;
}


void Automate2D::AppliquerTransition(const Etat& dep, Etat& dest) const{

    if (dep.getHauteur() < 2) //on vérifie que l'automate est 2D
        throw AutomateException("L'automate n'est pas 2D");

    if(dest.getHauteur()!=dep.getHauteur()||dest.getLargeur()!=dep.getLargeur()){
        dest=dep;
    }

    for(unsigned int i=0; i<dep.getLargeur(); i++){
        for(unsigned int j=0; j<dep.getHauteur(); j++){
            unsigned int nbVoisin=CountVoisin(i,j,dep);

            if (dep.getCellule(i,j)){ //cellule vivante
                if(nbVoisin<nbMinVivant || nbVoisin>nbMaxVivant)
                    dest.setCellule(i,j,false); //si trop ou pas assez de voisin alors la cellule meurt
            }

            else {

                if(nbVoisin>=nbMinMort && nbVoisin<=nbMaxMort)
                    dest.setCellule(i,j,true); //si suffisament de cellule alors la cellule nait*/
            }
            }
        }
}

std::ostream& operator<<(std::ostream& f, const Automate2D& A) {
	f <<"Regle cellule vivante :"<<endl<<"Nombre minimum = " << A.getMinV() <<endl<<"Nombre maximum"<<A.getMaxV()
	<<"Regle cellule morte :"<<endl<<"Nombre minimum = " << A.getMinM() <<endl<<"Nombre maximum"<<A.getMaxM();

	return f;
}

