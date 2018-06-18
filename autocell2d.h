#ifndef AUTOCELL2D_H
#define AUTOCELL2D_H

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
 * \brief The AutoCell2d class permet de gérer des automates jeu de la vie
 */
class AutoCell2d : public AutoCellmere{
Q_OBJECT
    /*!
     * \brief minV nombre min
     */
    QSpinBox* minV;
    /*!
     * \brief minVl Une cellule survit si son nombre de voisins vivant est compris entre
     */
    QLabel* minVl;
    /*!
     * \brief minH nombre min
     */
    QSpinBox* minH;
    /*!
     * \brief minHl Une cellule nait si son nombre de voisins vivant est compris entre
     */
    QLabel* minHl;
    QHBoxLayout* minVoc;
    /*!
     * \brief maxV nombre max
     */
    QSpinBox* maxV;
    QLabel* maxVl;
    QHBoxLayout* maxVoc;
    QHBoxLayout* minHoc;
    /*!
     * \brief maxH nombre max
     */
    QSpinBox* maxH;
    QLabel* maxHl;
    QHBoxLayout* maxHoc;
    /*!
     * \brief dimensionH dimension horizontale matrice
     */
    unsigned int dimensionH;
    /*!
     * \brief dimensionV dimension verticale matrice
     */
    unsigned int dimensionV;

public:
explicit AutoCell2d(QWidget* parent = nullptr,unsigned int i=10,unsigned int j=10);
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
#endif // AUTOCELL2D_H

