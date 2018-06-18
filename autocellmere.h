#ifndef AUTOCELLMERE_H
#define AUTOCELLMERE_H


#include <QThread>
#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include "automate.h"
#include "xml_dom.h"
#include "autocellmere.h"
/*!
 * \brief The AutoCellmere class est la classe mère dont hérite les 3 classes instanciant les automates
 */
class AutoCellmere : public QWidget{
Q_OBJECT
    protected:

    QSpinBox* clisteNor;
    QComboBox* liste;
    QComboBox* cliste;
    /*!
     * \brief lautomates  permet de charger un automate déjà sauvegardé
     */
    QComboBox* lautomates;
    QLabel* aff;
    QLabel* list;
    QLabel* clist;
    QLabel* clisteNorl;
    QVBoxLayout* listc;
    QVBoxLayout* clistc;
    QHBoxLayout* listoc;
    QHBoxLayout* clistoc;
    QHBoxLayout* testoc;
    QVBoxLayout* testc;
    QIntValidator* zeroOneValidator;
    QVBoxLayout* couche;
    /*!
     * \brief depart génére l'état de départ
     */
    QTableWidget* depart;
    /*!
     * \brief simulation lance la simulation
     */
    QPushButton* simulation;
    /*!
     * \brief etats genere la grille de simulation
     */
    QTableWidget*etats;
    /*!
     * \brief Quitter pour quitter l'application
     */
    QPushButton* Quitter;
    /*!
     * \brief b2 pour continuer la simulation
     */
    QPushButton* b2;
    /*!
     * \brief b3 Pour generer alétoirement un état de départ
     */
    QPushButton* b3;
    /*!
     * \brief b4 vide la grille
     */
    QPushButton* b4;
    /*!
     * \brief b5 reset
     */
    QPushButton* b5;
    /*!
     * \brief save sauvegarde un automate
     */
    QPushButton* save;
    /*!
     * \brief page instance de la classe Xml_Dom qui traite les fichiers xml
     */
    Xml_Dom *page;
    /*!
     * \brief b6 charge un automate sauvegardé
     */
    QPushButton* b6;

public:
    explicit AutoCellmere(QWidget* parent = nullptr):QWidget(parent){}
private slots:
    /*!
 * \brief cellActivation permet d'activer une cellule lorsque l'on clique dessus
 */
virtual void cellActivation(const QModelIndex& index)=0;
    /*!
 * \brief faireSimulation permet de lancer la simulation de l'automate
 */
virtual void faireSimulation()=0;
    /*!
 * \brief b2_clicked change un bouton
 */
virtual void b2_clicked()=0;
    /*!
 * \brief reset remet à zero l'état de départ
 */
virtual void reset()=0;
    /*!
 * \brief reset2 reset remet à zero l'état de départ et vide la grille de simulation
 */
virtual void reset2()=0;
    /*!
 * \brief genaleatoire genere une état de départ aléatoirement
 */
virtual void genaleatoire()=0;
    /*!
 * \brief b5_clicked reset
 */
virtual void b5_clicked()=0;
    /*!
 * \brief xml permet l'ajout d'un atomate dans le fichier xml
 */
virtual void xml()=0;
    /*!
 * \brief setNum permet de recuperer les valeurs de l'automate présent dans le fichier xml
 */
virtual void setNum()=0;

};
#endif // AUTOCELLMERE_H
