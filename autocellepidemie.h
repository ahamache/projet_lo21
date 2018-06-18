#ifndef AUTOCELLEPIDEMIE_H
#define AUTOCELLEPIDEMIE_H


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
#include "xml_dom.h"
#include "automate.h"
#include "autocellmere.h"
/*!
 * \brief The AutoCellepidemie class genere des atuomates epidemie
 */
class AutoCellepidemie : public AutoCellmere{
Q_OBJECT
    /*!
     * \brief c1 Etant malade, une cellule a une probabilité(sur /10) de mourir égale à:
     */
    QSpinBox* c1;
    QLabel* c1l;
    QHBoxLayout* c1oc;
    /*!
     * \brief c2 Etant saine, une cellule a une probabilité(sur /10) de tomber malade égale à:
     */
    QSpinBox* c2;
    QLabel* c2l;
    QHBoxLayout* c2oc;
    /*!
     * \brief dimensionH dimension horizontale de la matrice
     */
    unsigned int dimensionH;
    /*!
     * \brief dimensionV dimension verticale de la matrice
     */
    unsigned int dimensionV;

public:
explicit AutoCellepidemie(QWidget* parent = nullptr,unsigned int i=25,unsigned int j=25);
private slots:
void cellActivation(const QModelIndex& index);
void faireSimulation();
void b2_clicked();
void reset();
void reset2();
void genaleatoire();
void b5_clicked();
void xml();
void setNum();
};

#endif // AUTOCELLEPIDEMIE_H

