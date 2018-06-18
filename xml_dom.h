#ifndef XML_DOM_H
#define XML_DOM_H

#include <QtGui>
#include <QtXml>
#include <QWidget>
#include <QMessageBox>
#include "automate.h"

class Xml_Dom : public QWidget
{
    /*!
     * \brief xml_doc
     * objet dans lequel on charge le fichier XML
     */
    QFile xml_doc;
    /*!
     * \brief dom
     * objet auquel on attribut l'ensemble du fichier XML
     */
    QDomElement dom;
    /*!
     * \brief noeud
     * objet permettant de parcourir le premier noeud du fichier puis les autres noeuds frères
     */
    QDomNode noeud;
    /*!
     * \brief element
     * objet permettant de convertir l'objet noeud en un élément pour manipuler les données des balises
     */
    QDomElement element;
    /*!
     * \brief num
     * numero de la règle que l'on sauvegarde
     */
    unsigned int num;
    /*!
     * \brief numBit
     * numero en binaire de la règle que l'on sauvegarde
     */
    std::string numBit;
    /*!
     * \brief miniV
     * nombre minimum qu'il faut à une cellule vivante pour survivre
     */
    unsigned int miniV;
    /*!
     * \brief maxiV
     * nombre maximum qu'il faut à une cellule vivante pour survivre
     */
    unsigned int maxiV;
    /*!
     * \brief miniM
     * nombre minimum qu'il faut à une cellule morte pour survivre
     */
    unsigned int miniM;
    /*!
     * \brief maxiM
     * nombre maximum qu'il faut à une cellule morte pour survivre
     */
    unsigned int maxiM;
    /*!
     * \brief c1
     * chance de mourir en étant malade
     */
    unsigned int c1;
    /*!
     * \brief c2
     * chance de tomber malade en étant sain avec un voisin malade
     */
    unsigned int c2;
    //const Automate1D& A;
public:
    /*!
     * \brief Xml_Dom
     * \param param : type d'automate
     * constructeur de la classe qui lit le fichier XML et construit les automates correspondant dans l'AutomateManager
     */
    Xml_Dom(const QString& param);
    ~Xml_Dom(){}
    //void ajouter_Automate();
    /*!
     * \brief ajouter_Automate1D
     * \param n : numéro de la règle
     * Permet d'ajouter un nouvel automate dont le numéro de la règle est transmise en paramètre
     */
    void ajouter_Automate1D(unsigned int n);
    /*!
     * \brief ajouter_Automate2D
     * \param mnV : minimum pour une cellule vivante
     * \param mxV : maximum pour une cellule vivante
     * \param mnM : minimum pour une cellule morte
     * \param mxM : maximum pour une cellule morte
     * Permet d'ajouter un nouvel automate avec les attributs transmis en paramètre
     */
    void ajouter_Automate2D(unsigned int mnV, unsigned int mxV, unsigned int mnM, unsigned int mxM);
    /*!
     * \brief ajouter_AutomateEp
     * \param c1 : chance de mourir en étant malade
     * \param c2 : chance de tomber malade en étant sain
     * Permet d'ajouter un nouvel automate avec les attributs transmis en paramètre
     */
    void ajouter_AutomateEp(unsigned int c1, unsigned int c2);

};

#endif // XML_DOM_H

