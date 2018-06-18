#include "etat.h"

Etat::Etat(unsigned int l, unsigned int c): nbColonne(c), nbLigne(l){

    if (c==0 || l==0)   /*! On vérifie que le nombre de colonnes et le nombre de lignes sont non nuls*/
        throw EtatException("Mauvaise dimension de la matrice");

    else{
        valeurs = new int*[nbLigne]; /*! On crée la grille de cellules par allocation dynamique*/
        for (unsigned int i=0; i<nbLigne; i++){
            valeurs[i] = new int[nbColonne];
            for (unsigned int j = 0; j < nbColonne; ++j)
                valeurs[i][j] = 0;  /*! Par défaut, toutes les cellules sont à l'état 0*/
                }
    }
}

void Etat::setCellule(unsigned int i, unsigned int j, int val) {
    if (i >= nbLigne || j>=nbColonne)   /*!< On vérifie que les numéros de ligne et de colonne entrés en paramètre ne sont pas supérieurs aux dimensions de la grille */
        throw EtatException("Cellule inexistante dimension trop élevée");
    valeurs[i][j] = val; /*! On modifie la valeur de la cellule correspondante*/
}

int Etat::getCellule(unsigned int i, unsigned int j) const{
    if (i>=nbLigne|| j>=nbColonne) /*! On vérifie que les numéros de ligne et de colonne entrés en paramètre ne sont pas supérieurs aux dimensions de la grille */
        throw EtatException("Cellule inexistante");
    return valeurs[i][j];   /*! On renvoie la valeur de la cellule correspondante*/
}

Etat::~Etat(){
/*! Désallocation dynamique de l'attribut valeurs*/
for(unsigned int i=0; i<nbLigne;++i)
        delete valeurs[i];
    delete[] valeurs;
}

Etat::Etat(const Etat& e): nbColonne(e.nbColonne), nbLigne(e.nbLigne), valeurs(nullptr){

    if(nbLigne<=0 || nbColonne<=0)
        throw EtatException("Mauvaise dimension");

    else {
        valeurs = new int*[nbLigne];
        for(unsigned int i=0; i<nbLigne; ++i){

            valeurs[i]=new int[nbColonne];
            for(unsigned int j=0; j<nbColonne; ++j){
                valeurs[i][j]=e.valeurs[i][j];
            }
        }
    }

}

Etat& Etat::operator=(const Etat& e){
    if(this != &e){ /*! On vérifie qu'il ne s'agit pas d'une auto-affectation*/
        if(nbLigne!=e.nbLigne){ /*! on recrée tout le tableau valeurs si les grilles n'ont pas la même dimension*/

            for(unsigned int i=0;i<nbLigne;++i)
                delete valeurs[i]; //on supprime chaque ligne
            delete[] valeurs;

            nbLigne=e.nbLigne;
            valeurs=new int*[nbLigne]; //on reconstruit un tableau de pointeurs
            nbColonne=e.nbColonne;

            for(unsigned int i=0; i<nbLigne;++i)
                valeurs[i]=new int[nbColonne];
            }

        else if(nbColonne!=e.nbColonne){ //il y a le bon nombre de ligne alors on reconstruit seulement les lignes

            nbColonne=e.nbColonne;
            for(unsigned int i=0;i<nbLigne;++i){
                delete valeurs[i];
                valeurs[i]=new int[nbColonne];
            }
        }

        for (unsigned int i=0;i<nbLigne;i++){
            for(unsigned int j=0;j<nbColonne;++j)
                valeurs[i][j]=e.valeurs[i][j];
        }
    }

    return *this;
    }

std::ostream& operator<<(std::ostream& f, const Etat& e){

    for (unsigned int i=0; i<e.getLongueur(); ++i){
        for (unsigned int j=0; j<e.getLargeur(); ++j)
            f<<e.getCellule(i,j);
        f<<endl;
    }
    return f;

}
