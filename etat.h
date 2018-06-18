#ifndef ETAT_H_INCLUDED
#define ETAT_H_INCLUDED

#include <string>
#include <iostream>
using namespace std;

/*!
 * \class EtatException
 * \brief Gère les exceptions générées dans la classe Etat
 *
 */
class EtatException {
public:
    EtatException(const std::string& message):info(message) {}
    std::string getInfo() const { return info; }
private:
    std::string info;
};

/*! \class Etat
  * \brief Classe représentant les grilles cellulaires
  *
  */
class Etat{

    unsigned int nbColonne; /*!< Nombre de colonnes de la grille>*/
    unsigned int nbLigne; /*!< Nombre de lignes de la grille. Si la grille est à une dimension, cet attribut vaut 1.>*/
    int** valeurs;

public:
    /*!
         *  \brief Constructeur
         *
         *  Constructeur de la classe Etat
         *
         *  \param c : nombre de colonnes
         *  \param l : nombre de lignes
         */
    Etat(unsigned int c, unsigned int l);
    /*!
         *  \brief Constructeur
         *
         *  Constructeur de la classe Etat
         */
    Etat() : nbColonne(0), nbLigne(0), valeurs(nullptr) {}
    /*!
     * \brief getLargeur
     * \return Retourne le nombre de colonnes de la grille
     */
    unsigned int getLargeur()const {return nbColonne;}
    /*!
     * \brief getLongueur
     * \return Retourne le nombre de lignes de la grille
     */
    unsigned int getLongueur()const {return nbLigne;}
     /*!< \brief Destructeur de la classe Etat */
    ~Etat();
    /*!
         * Retourne l'état de la cellule de la grille aux coordonnées (i,j)
         */
    int getCellule(unsigned int i, unsigned int j) const;
    /*!
         * Permet de modifier l'état de la cellule de la grille aux coordonnées (i,j)
         */
    void setCellule(unsigned int i, unsigned int j, int val);
     /*! Constructeur par recopie*/
    Etat(const Etat& e);
     /*! Opérateur = redéfini pour permettre une affectation entre états */
    Etat& operator=(const Etat& e);

};
/*!
 * \brief operator <<
 * \param f flux de sortie
 * \param e état à afficher
 */
std::ostream& operator<<(std::ostream& f, const Etat& e);

#endif // ETAT_H_INCLUDED
