#include "autocell2d.h"
#include "simulateur.h"
#include"xml_dom.h"
#include <windows.h>
#include<QApplication>

bool btnContinuer2 = false;
bool btnReset2 = false;
/*!
 * \brief AutoCell2d::AutoCell2d constructeur de la page
 * \param parent
 * \param i dimension horizontale de la matrice
 * \param j dimension verticale de la matrice
 */
AutoCell2d::AutoCell2d(QWidget *parent, unsigned int i,unsigned int j):AutoCellmere(parent), dimensionH(i), dimensionV(j) {

    page = new Xml_Dom(QString::fromStdString("Automate2D"));
    aff=new QLabel("Charger un automate déjà sauvegardé : ");
    lautomates = new QComboBox(this);
    testc=new QVBoxLayout;
    testc->addWidget(aff);
    testc->addWidget(lautomates);
    testoc=new QHBoxLayout;
    testoc->addLayout(testc);

    for(unsigned int i=0; i<AutomateManager::getInstance().getNb2DStockes(); i++){ //on inspecte l'ensemble du tableau pour voir si un automate existe déjà

        if(AutomateManager::getInstance().getElem2D(i)!=nullptr){ //si la regle a deja ete rentree dans le tableau d'automates1D[] d'AutomateManager
            QString s= ("MinVivant=" + QString::number(AutomateManager::getInstance().getElem2D(i)->getMinV()) + "    MaxVivant=" + QString::number(AutomateManager::getInstance().getElem2D(i)->getMaxV()) + "    MinMort=" + QString::number(AutomateManager::getInstance().getElem2D(i)->getMinM()) + "    MaxMort=" + QString::number(AutomateManager::getInstance().getElem2D(i)->getMaxM()) );
            lautomates->addItem(s);
        }
    }

    b6 = new QPushButton("Charger l'automate");
    connect(b6,SIGNAL(clicked()),this,SLOT(setNum()));


    list=new QLabel("Choix de l'initialisation:");
    liste = new QComboBox(this);
    liste->addItem("Vide");
    liste->addItem("Aleatoire");
    liste->addItem("Symetrie axe horizontal");
    liste->addItem("Symetrie axe vertical");
    liste->addItem("Symetrie centrée");
    listc=new QVBoxLayout;
    listc->addWidget(list);
    listc->addWidget(liste);

    listoc=new QHBoxLayout;
    listoc->addLayout(listc);
    clist=new QLabel("Choix du mode de déroulement:");
    cliste = new QComboBox(this);
    cliste->addItem("Pas à Pas");
    cliste->addItem("Normal avec pauses");
    clisteNor= new QSpinBox(this);
    clisteNor->setRange(1,5);
    clisteNor->setValue(3);
    clisteNorl=new QLabel("Temps de pause");
    clistc=new QVBoxLayout;
    clistc->addWidget(clist);
    clistc->addWidget(cliste);
    clistc->addWidget(clisteNorl);
    clistc->addWidget(clisteNor);

    clistoc=new QHBoxLayout;
    clistoc->addLayout(clistc);

    b2=new QPushButton("Continuer Simulation(Mode pas à pas)");
    b3=new QPushButton("Genereration Aléatoire(Uniquement si activé)");
    b4=new QPushButton("Vider la grille(Uniquement si activé)");
    b5=new QPushButton("Reset/Arret");

    minV= new QSpinBox(this);
    minV->setRange(0,8);
    maxV= new QSpinBox(this);
    maxV->setRange(0,8);
    minVl=new QLabel("Une cellule survit si son nombre de voisins vivant est compris entre");
    maxVl=new QLabel("et");
    minVoc=new QHBoxLayout;
    minVoc->addWidget(minVl);
    minVoc->addWidget(minV);
    minVoc->addWidget(maxVl);
    minVoc->addWidget(maxV);

    minH= new QSpinBox(this);
    minH->setRange(0,8);
    maxH= new QSpinBox(this);
    maxH->setRange(0,8);
    minHl=new QLabel("Une cellule nait si son nombre de voisins vivant est compris entre");
    maxHl=new QLabel("et");
    minHoc=new QHBoxLayout;
    minHoc->addWidget(minHl);
    minHoc->addWidget(minH);
    minHoc->addWidget(maxHl);
    minHoc->addWidget(maxH);
/*! recupere le dernier automate chargé*/
    if(AutomateManager::getInstance().getNb2DStockes()==0){
        minV->setValue(2);
        maxV->setValue(3);
        minH->setValue(3);
        maxH->setValue(3);
    }
    else {
        minV->setValue(AutomateManager::getInstance().getElem2D(AutomateManager::getInstance().getNb2DStockes()-1)->getMinV());
        maxV->setValue(AutomateManager::getInstance().getElem2D(AutomateManager::getInstance().getNb2DStockes()-1)->getMaxV());
        minH->setValue(AutomateManager::getInstance().getElem2D(AutomateManager::getInstance().getNb2DStockes()-1)->getMinM());
        maxH->setValue(AutomateManager::getInstance().getElem2D(AutomateManager::getInstance().getNb2DStockes()-1)->getMaxM());
    }

    save=new QPushButton("Sauvegarder l'automate");

    Quitter=new QPushButton("Quitter");

    couche=new QVBoxLayout;
    couche->addLayout(testoc);
    couche->addWidget(b6);
    couche->addLayout(listoc);
    couche->addLayout(clistoc);
    couche->addLayout(minVoc);
    couche->addLayout(minHoc);
    couche->addWidget(save);


    depart=new QTableWidget(dimensionV,dimensionH,this);
    couche->addWidget(depart);
    couche->addWidget(b3);
    couche->addWidget(b4);
    setLayout(couche);

/*! initialisation état de départ*/
    unsigned int taille=18;
    depart->setFixedSize(dimensionH*taille,dimensionV*taille);
    depart->horizontalHeader()->setVisible(false);
    depart->verticalHeader()->setVisible(false);
    depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    if(liste->currentIndex()==0){
    for(unsigned int i=0;i<dimensionH;i++){
        depart->setColumnWidth(i,taille);
        depart->setRowHeight(i,taille);
        for(unsigned int j=0;j<dimensionV;j++){
            depart->setItem(j,i,new QTableWidgetItem(""));
            }
        }
    }

/*! initialisation mode aléatoire*/
    if(liste->currentIndex()==1){
        for(unsigned int i=0; i<dimensionH;i++){
            depart->setColumnWidth(i,taille);
            for(unsigned int j=0; j<dimensionV;j++){
                depart->setRowHeight(j,taille);
                unsigned int a=rand()%2;
                if(a==0){
                    depart->setItem(i,j,new QTableWidgetItem(""));
                }
                else{
                    depart->setItem(i,j,new QTableWidgetItem("_"));
                 }
            }
        }
   }
   if(liste->currentIndex()==2){
       for(unsigned int i=0; i<dimensionH;i++){
           depart->setColumnWidth(i,taille);
           for(unsigned int j=0; j<dimensionV;j++){
               depart->setRowHeight(j,taille);
               depart->setItem(i,j,new QTableWidgetItem(""));
           }
       }
   }
   connect(depart,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(cellActivation(QModelIndex)));
   connect(Quitter,SIGNAL(clicked()),qApp,SLOT(quit()));
   connect(save,SIGNAL(clicked()),this,SLOT(xml()));
   connect(b2,SIGNAL(clicked()),this,SLOT(b2_clicked()));
   connect(b4,SIGNAL(clicked()),this,SLOT(reset()));
   connect(b3,SIGNAL(clicked()),this,SLOT(genaleatoire()));
   connect(b5,SIGNAL(clicked()),this,SLOT(b5_clicked()));
   connect(b5,SIGNAL(clicked()),this,SLOT(reset2()));

   simulation=new QPushButton("Simulation");
   couche->addWidget(simulation);
   connect(simulation,SIGNAL(clicked()),this,SLOT(faireSimulation()));
   etats=new QTableWidget(dimensionV,dimensionH,this);
   etats->setFixedSize(dimensionH*taille,dimensionV*taille);
   etats->horizontalHeader()->setVisible(false);
   etats->verticalHeader()->setVisible(false);
   etats->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   etats->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   //non editable
   etats->setEditTriggers(QAbstractItemView::NoEditTriggers);
   for(unsigned int i=0;i<dimensionH;i++){
       etats->setColumnWidth(i,taille);
       etats->setRowHeight(i,taille);
       for(unsigned int j=0;j<dimensionV;j++){
           etats->setItem(j,i,new QTableWidgetItem(""));
       }
   }
   couche->addWidget(etats);
   couche->addWidget(b5);
   couche->addWidget(b2);
   couche->addWidget(Quitter);

}

/*!
 * \brief AutoCell2d::faireSimulation réalise la simulation d'un automate
 */
void AutoCell2d::faireSimulation(){
    ::btnReset2 = false;
    Etat e(dimensionV,dimensionH);
    for(unsigned int i=0; i<dimensionV;i++){
        for(unsigned int j=0; j<dimensionH;j++){
            if(depart->item(i,j)->text()!="")e.setCellule(i,j,1);
        }
    }
    const Automate2D& A= AutomateManager::getInstance().getAutomate2D(minV->value(),maxV->value(),minH->value(),maxH->value());
    Simulateur S(A,e);
    if(cliste->currentIndex()==0){
        /*! 3e boucle ici nécessaire pour initialiser les maximum 100 états*/
        for(unsigned int k=0; k<100; k++){
            S.next();
            const Etat& d=S.dernier();
            for(unsigned int i=0; i<dimensionV; i++){
                for(unsigned int j=0; j<dimensionH; j++){
                    if(d.getCellule(i,j)==1){
                        etats->item(i,j)->setBackgroundColor("black");
                    }
                    else{
                        etats->item(i,j)->setBackgroundColor("white");

                    }
                }
            }
                   while ( ::btnContinuer2 != true ){
                       QThread::msleep(20);
                      QCoreApplication::processEvents();
                      if(::btnReset2 == true){
                       return;
                      }

                }
          ::btnContinuer2 = false;
            }


        }
    else{
        for(unsigned int k=0; k<100; k++){
        S.next();
        if(cliste->currentIndex()==1){
            if(::btnReset2 == true){
             return;
            }
            unsigned int a=clisteNor->value();
             QThread::msleep(a*100);
             QCoreApplication::processEvents();

        }
        const Etat& d=S.dernier();
        for(unsigned int i=0; i<dimensionV; i++){
            for(unsigned int j=0; j<dimensionH; j++){
                if(d.getCellule(i,j)==1){
                    etats->item(i,j)->setBackgroundColor("black");
                }
                else{
                    etats->item(i,j)->setBackgroundColor("white");

                }
            }
        }
     }
   }
}
/*!
 * \brief AutoCell2d::b2_clicked changement du bouton pour continuer simulation
 */
void AutoCell2d::b2_clicked(){
    ::btnContinuer2 = true;
}
/*!
 * \brief AutoCell2d::genaleatoire generation aléatoire
 */

void AutoCell2d::genaleatoire(){
    if(liste->currentIndex()==1){
        for(unsigned int i=0; i<dimensionV;i++){
            for(unsigned int j=0; j<dimensionH;j++){
            unsigned int a=rand()%2;
            if(a==0){
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("white");
                depart->item(i,j)->setTextColor("white");
            }
            else{
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("black");
                depart->item(i,j)->setTextColor("black");
            }
         }
      }
   }
}

/*!
 * \brief AutoCell2d::reset reset de la grille si mode activé
 */
void AutoCell2d::reset(){
    if(liste->currentIndex()==0){
        for(unsigned int i=0; i<dimensionV;i++){
             for(unsigned int j=0; j<dimensionH;j++){
                 depart->item(i,j)->setText("");
                 depart->item(i,j)->setBackgroundColor("white");
                 depart->item(i,j)->setTextColor("white");
             }
        }
    }
}
/*!
 * \brief AutoCell2d::reset2 reset lors de l'arrêt
 */
void AutoCell2d::reset2(){
    for(unsigned int i=0; i<dimensionV;i++){
         for(unsigned int j=0; j<dimensionH;j++){
             depart->item(i,j)->setText("");
             depart->item(i,j)->setBackgroundColor("white");
             depart->item(i,j)->setTextColor("white");

        }
    }
}

/*!
 * \brief AutoCell2d::b5_clicked réinitialisation grille de simulation
 */
void AutoCell2d::b5_clicked(){
    unsigned int taille=18;
    for(unsigned int i=0; i<dimensionV;i++){
        etats->setColumnWidth(i,taille);
        for(unsigned int j=0; j<dimensionH;j++){
            etats->setRowHeight(j,taille);
            etats->setItem(i,j,new QTableWidgetItem(""));
        }
    }


    ::btnReset2 = true;
}

/*!
 * \brief AutoCell2d::cellActivation change l'etat d'une cellule lorsque l'on clique dessus
 */

void AutoCell2d::cellActivation(const QModelIndex& index){
    if (depart->item(index.row(),index.column())->text()==""){//desactivé
        depart->item(index.row(),index.column())->setText("_");
        depart->item(index.row(),index.column())->setBackgroundColor("black");
        depart->item(index.row(),index.column())->setTextColor("black");
        if(liste->currentIndex()==2){
            depart->item(index.row(),dimensionH-1-index.column())->setText("_");
            depart->item(index.row(),dimensionH-1-index.column())->setBackgroundColor("black");
            depart->item(index.row(),dimensionH-1-index.column())->setTextColor("black");

        }
        if(liste->currentIndex()==3){
            depart->item(dimensionV-1-index.row(),index.column())->setText("_");
            depart->item(dimensionV-1-index.row(),index.column())->setBackgroundColor("black");
            depart->item(dimensionV-1-index.row(),index.column())->setTextColor("black");

        }
        if(liste->currentIndex()==4){
            depart->item(dimensionV-1-index.row(),dimensionH-1-index.column())->setText("_");
            depart->item(dimensionV-1-index.row(),dimensionH-1-index.column())->setBackgroundColor("black");
            depart->item(dimensionV-1-index.row(),dimensionH-1-index.column())->setTextColor("black");

        }
    }
    else{ //activé
        depart->item(index.row(),index.column())->setText("");
        depart->item(index.row(),index.column())->setBackgroundColor("white");
        depart->item(index.row(),index.column())->setTextColor("white");
        if(liste->currentIndex()==2){
            depart->item(index.row(),dimensionH-1-index.column())->setText("");
            depart->item(index.row(),dimensionH-1-index.column())->setBackgroundColor("white");
            depart->item(index.row(),dimensionH-1-index.column())->setTextColor("white");

        }
        if(liste->currentIndex()==3){
            depart->item(dimensionV-1-index.row(),index.column())->setText("");
            depart->item(dimensionV-1-index.row(),index.column())->setBackgroundColor("white");
            depart->item(dimensionV-1-index.row(),index.column())->setTextColor("white");

        }
        if(liste->currentIndex()==4){
            depart->item(dimensionV-1-index.row(),dimensionH-1-index.column())->setText("");
            depart->item(dimensionV-1-index.row(),dimensionH-1-index.column())->setBackgroundColor("white");
            depart->item(dimensionV-1-index.row(),dimensionH-1-index.column())->setTextColor("white");

        }
    }
}
/*!
 * \brief AutoCell2d::xml initialisation de l'xml
 */
void AutoCell2d::xml(){
    page->ajouter_Automate2D(minV->value(), maxV->value(), minH->value(), maxH->value());
}
/*!
 * \brief AutoCell2d::setNum recuperation des valeurs du fichier xml pour initialisation
 */
void AutoCell2d::setNum() {
    minV->setValue(lautomates->currentText()[10].digitValue());
    maxV->setValue(lautomates->currentText()[25].digitValue());
    minH->setValue(lautomates->currentText()[38].digitValue());
    maxH->setValue(lautomates->currentText()[51].digitValue());
}
