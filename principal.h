#ifndef PRINCIPAL_H
#define PRINCIPAL_H


#include <QWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
/*!
 * \brief The Automain class permet d'afficher le menu principal
 */
class Automain : public QWidget{
    Q_OBJECT
    QVBoxLayout* c1;
    /*!
     * \brief a1d boutons qui lient l'appel aux différents automates
     */
    QPushButton* a1d;
    QPushButton* a2d;
    QPushButton* a3d;
    /*!
     * \brief lon taille de la grille
     */
    QSpinBox* lon;
    QVBoxLayout* lonc;
    QHBoxLayout* lonoc;
    QLabel* lonl;
    /*!
     * \brief dlon taille de la grille
     */
    QSpinBox* dlon;
    QVBoxLayout* dlonc;
    QHBoxLayout* dlonoc;
    QLabel* dlonl;
    /*!
     * \brief clon taille de la grille
     */
    QSpinBox* clon;
    QVBoxLayout* clonc;
    QHBoxLayout* clonoc;
    QLabel* clonl;
    /*!
     * \brief d3lon taille de la grille
     */
    QSpinBox* d3lon;
    QVBoxLayout* d3lonc;
    QHBoxLayout* d3lonoc;
    QLabel* d3lonl;
    /*!
     * \brief c3lon taille de la grille
     */
    QSpinBox* c3lon;
    QVBoxLayout* c3lonc;
    QHBoxLayout* c3lonoc;
    QLabel* c3lonl;
    /*!
     * \brief Quitter Bouton Quitter
     */
    QPushButton* Quitter;
    QLabel* label;
    unsigned int i;

public:
    explicit Automain(QWidget* parent = nullptr);
private slots:
    /*!
  * \brief lancerauto1d lancer automate 1D
  */
 void lancerauto1d();
 /*!
  * \brief lancerauto2d lancer automate 2D
  */
 void lancerauto2d();
 /*!
  * \brief lancerauto3 lancer automate Epidemie
  */
 void lancerauto3();
};



#endif // PRINCIPAL_H

