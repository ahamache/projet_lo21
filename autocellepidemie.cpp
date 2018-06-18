#include "autocellepidemie.h"
#include "simulateur.h"
#include"xml_dom.h"
#include <windows.h>
#include<QApplication>

bool btnContinuer3 = false;
bool btnReset3 = false;
/*!
 * \brief AutoCellepidemie::AutoCellepidemie gere des automates Epidemie
 * \param parent
 * \param i dimension horizontale
 * \param j dimension verticale
 */
AutoCellepidemie::AutoCellepidemie(QWidget *parent, unsigned int i,unsigned int j):AutoCellmere(parent), dimensionH(i), dimensionV(j) {

    page = new Xml_Dom(QString::fromStdString("AutomateEpidemie"));
    aff=new QLabel("Charger un automate déjà sauvegardé : ");
    lautomates = new QComboBox(this);
    testc=new QVBoxLayout;
    testc->addWidget(aff);
    testc->addWidget(lautomates);
    testoc=new QHBoxLayout;
    testoc->addLayout(testc);

    for(unsigned int i=0; i<AutomateManager::getInstance().getNbEpStockes(); i++){ //on inspecte l'ensemble du tableau pour voir si un automate existe déjà
        if(AutomateManager::getInstance().getElemEp(i)!=nullptr){ //si la regle a deja ete rentree dans le tableau d'automates1D[] d'AutomateManager
            QString s= ("Chance1=" + QString::number(AutomateManager::getInstance().getElemEp(i)->getChance1()) + "    Chance2=" + QString::number(AutomateManager::getInstance().getElemEp(i)->getChance2()));
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
    clistc->addWidget(clisteNor);
    clistc->addWidget(clisteNorl);

    clistoc=new QHBoxLayout;
    clistoc->addLayout(clistc);

    b2=new QPushButton("Continuer Simulation(Mode pas à pas)");
    b3=new QPushButton("Genereration Aléatoire(Uniquement si activé)");
    b4=new QPushButton("Vider la grille(Uniquement si activé)");
    b5=new QPushButton("Reset/Arret");

    c1= new QSpinBox(this);
    c1->setRange(0,10);
    c1l=new QLabel("Etant malade, une cellule a une probabilité(sur /10) de mourir égale à:");
    c1oc=new QHBoxLayout;
    c1oc->addWidget(c1l);
    c1oc->addWidget(c1);


    c2= new QSpinBox(this);
    c2->setRange(0,10);
    c2l=new QLabel("Etant saine, une cellule a une probabilité(sur /10) de tomber malade égale à:");
    c2oc=new QHBoxLayout;
    c2oc->addWidget(c2l);
    c2oc->addWidget(c2);
    /*! recupere les valeurs du dernier automate chargé*/
    if(AutomateManager::getInstance().getNbEpStockes()==0){
        c1->setValue(5);
        c2->setValue(5);
    }
    else {
        c1->setValue(AutomateManager::getInstance().getElemEp(AutomateManager::getInstance().getNbEpStockes()-1)->getChance1());
        c2->setValue(AutomateManager::getInstance().getElemEp(AutomateManager::getInstance().getNbEpStockes()-1)->getChance2());
    }

    save=new QPushButton("Sauvegarder l'automate");

    Quitter=new QPushButton("Quitter");

    couche=new QVBoxLayout;
    couche->addLayout(testoc);
    couche->addWidget(b6);
    couche->addLayout(listoc);
    couche->addLayout(clistoc);
    couche->addLayout(c1oc);
    couche->addLayout(c2oc);
    couche->addWidget(save);

    depart=new QTableWidget(dimensionV,dimensionH,this);
    couche->addWidget(depart);
    couche->addWidget(b3);
    couche->addWidget(b4);
    setLayout(couche);


    unsigned int taille=10;
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
            depart->item(j,i)->setBackgroundColor("green");
            depart->item(j,i)->setTextColor("green");
            }
        }
    }


    if(liste->currentIndex()==1){
        for(unsigned int i=0; i<dimensionH;i++){
            depart->setColumnWidth(i,taille);
            for(unsigned int j=0; j<dimensionV;j++){
                depart->setRowHeight(j,taille);
                unsigned int a=rand()%100;
                if(a>2){
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
 * \brief AutoCellepidemie::faireSimulation réalise la simulation d'un automate
 */
void AutoCellepidemie::faireSimulation(){
    ::btnReset3 = false;
    Etat e(dimensionV,dimensionH);
    for(unsigned int i=0; i<dimensionV;i++){
        for(unsigned int j=0; j<dimensionH;j++){
            if(depart->item(i,j)->text()!="")e.setCellule(i,j,1);
        }
    }
    const AutomateEpidemie& A= AutomateManager::getInstance().getAutomateEp(c1->value(),c2->value());
    Simulateur S(A,e);
    if(cliste->currentIndex()==0){
        for(unsigned int k=0; k<100; k++){
            S.next();
            const Etat& d=S.dernier();
            for(unsigned int i=0; i<dimensionV; i++){
                /*! ici 4 couleurs différentes selon l'etat de la cellule*/
                for(unsigned int j=0; j<dimensionH; j++){
                    if(d.getCellule(i,j)==0){
                        /*! saine*/
                        etats->item(i,j)->setBackgroundColor("green");
                    }
                    if(d.getCellule(i,j)==1){
                        /*! malade*/
                        etats->item(i,j)->setBackgroundColor("red");
                    }
                    if(d.getCellule(i,j)==2){
                        /*! immunisé*/
                        etats->item(i,j)->setBackgroundColor("blue");
                    }
                    if(d.getCellule(i,j)==3){
                        /*! morte*/
                        etats->item(i,j)->setBackgroundColor("grey");
                    }

                }
            }
                   while ( ::btnContinuer3 != true ){
                       QThread::msleep(20);
                      QCoreApplication::processEvents();
                      if(::btnReset3 == true){
                       return;
                      }

                }
          ::btnContinuer3 = false;
            }


        }
    else{
        for(unsigned int k=0; k<100; k++){
        S.next();
        if(cliste->currentIndex()==1){
            if(::btnReset3 == true){
             return;
            }
            unsigned int a=clisteNor->value();
             QThread::msleep(a*100);
             QCoreApplication::processEvents();

        }
        const Etat& d=S.dernier();
        for(unsigned int i=0; i<dimensionV; i++){
            for(unsigned int j=0; j<dimensionH; j++){
                if(d.getCellule(i,j)==0){
                    etats->item(i,j)->setBackgroundColor("green");
                }
                if(d.getCellule(i,j)==1){
                    etats->item(i,j)->setBackgroundColor("red");
                }
                if(d.getCellule(i,j)==2){
                    etats->item(i,j)->setBackgroundColor("blue");
                }
                if(d.getCellule(i,j)==3){
                    etats->item(i,j)->setBackgroundColor("grey");
                }

            }
        }
     }
   }
}
/*!
 * \brief AutoCellepidemie::b2_clicked changement du bouton pour continuer simulation
 */
void AutoCellepidemie::b2_clicked(){
    ::btnContinuer3 = true;
}
/*!
 * \brief AutoCellepidemie::genaleatoire generation aléatoire
 */
void AutoCellepidemie::genaleatoire(){
    if(liste->currentIndex()==1){
        for(unsigned int i=0; i<dimensionV;i++){
            for(unsigned int j=0; j<dimensionH;j++){
            unsigned int a=rand()%100;
            if(a>2){
                depart->item(i,j)->setText("");
                depart->item(i,j)->setBackgroundColor("green");
                depart->item(i,j)->setTextColor("green");
            }
            else{
                depart->item(i,j)->setText("_");
                depart->item(i,j)->setBackgroundColor("red");
                depart->item(i,j)->setTextColor("red");
            }
         }
      }
   }
}

/*!
 * \brief AutoCellepidemie::reset reset de la grille si mode activé
 */
void AutoCellepidemie::reset(){
    if(liste->currentIndex()==0){
        for(unsigned int i=0; i<dimensionV;i++){
             for(unsigned int j=0; j<dimensionH;j++){
                 depart->item(i,j)->setText("");
                 depart->item(i,j)->setBackgroundColor("green");
                 depart->item(i,j)->setTextColor("green");
             }
        }
    }
}
/*!
 * \brief AutoCellepidemie::reset2 reset lors de l'arrêt
 */
void AutoCellepidemie::reset2(){
    for(unsigned int i=0; i<dimensionV;i++){
         for(unsigned int j=0; j<dimensionH;j++){
             depart->item(i,j)->setText("");
             depart->item(i,j)->setBackgroundColor("green");
             depart->item(i,j)->setTextColor("green");

        }
    }
}

/*!
 * \brief AutoCellepidemie::b5_clicked réinitialisation grille de simulation
 */
void AutoCellepidemie::b5_clicked(){
    unsigned int taille=10;
    for(unsigned int i=0; i<dimensionV;i++){
        etats->setColumnWidth(i,taille);
        for(unsigned int j=0; j<dimensionH;j++){
            etats->setRowHeight(j,taille);
            etats->setItem(i,j,new QTableWidgetItem(""));
        }
    }


    ::btnReset3 = true;
}


/*!
 * \brief AutoCellepidemie::cellActivation  change l'etat d'une cellule lorsque l'on clique dessus
 */
void AutoCellepidemie::cellActivation(const QModelIndex& index){
    if (depart->item(index.row(),index.column())->text()==""){//desactivé
        depart->item(index.row(),index.column())->setText("_");
        depart->item(index.row(),index.column())->setBackgroundColor("red");
        depart->item(index.row(),index.column())->setTextColor("red");
        if(liste->currentIndex()==2){
            depart->item(index.row(),dimensionH-1-index.column())->setText("_");
            depart->item(index.row(),dimensionH-1-index.column())->setBackgroundColor("red");
            depart->item(index.row(),dimensionH-1-index.column())->setTextColor("red");

        }
        if(liste->currentIndex()==3){
            depart->item(dimensionV-1-index.row(),index.column())->setText("_");
            depart->item(dimensionV-1-index.row(),index.column())->setBackgroundColor("red");
            depart->item(dimensionV-1-index.row(),index.column())->setTextColor("red");

        }
        if(liste->currentIndex()==4){
            depart->item(dimensionV-1-index.row(),dimensionH-1-index.column())->setText("_");
            depart->item(dimensionV-1-index.row(),dimensionH-1-index.column())->setBackgroundColor("red");
            depart->item(dimensionV-1-index.row(),dimensionH-1-index.column())->setTextColor("red");

        }
    }
    else{ //activé
        depart->item(index.row(),index.column())->setText("");
        depart->item(index.row(),index.column())->setBackgroundColor("green");
        depart->item(index.row(),index.column())->setTextColor("green");
        if(liste->currentIndex()==2){
            depart->item(index.row(),dimensionH-1-index.column())->setText("");
            depart->item(index.row(),dimensionH-1-index.column())->setBackgroundColor("green");
            depart->item(index.row(),dimensionH-1-index.column())->setTextColor("green");

        }
        if(liste->currentIndex()==3){
            depart->item(dimensionV-1-index.row(),index.column())->setText("");
            depart->item(dimensionV-1-index.row(),index.column())->setBackgroundColor("green");
            depart->item(dimensionV-1-index.row(),index.column())->setTextColor("green");

        }
        if(liste->currentIndex()==4){
            depart->item(dimensionV-1-index.row(),dimensionH-1-index.column())->setText("");
            depart->item(dimensionV-1-index.row(),dimensionH-1-index.column())->setBackgroundColor("green");
            depart->item(dimensionV-1-index.row(),dimensionH-1-index.column())->setTextColor("green");

        }
    }
}
/*!
 * \brief AutoCellepidemie::xml initialisation de l'xml
 */
void AutoCellepidemie::xml(){
    page->ajouter_AutomateEp(c1->value(), c2->value());
}

/*!
 * \brief AutoCellepidemie::setNum  recuperation des valeurs du fichier xml pour initialisation
 */

void AutoCellepidemie::setNum() {
    if(lautomates->currentText()[9]=='0'){
        c1->setValue(10);
        if(lautomates->currentText()[23]=='0')
            c2->setValue(10);
        else
            c2->setValue(lautomates->currentText()[22].digitValue());
    }
    else{
        c1->setValue(lautomates->currentText()[8].digitValue());
        if(lautomates->currentText()[22]=='0')
            c2->setValue(10);
        else
            c2->setValue(lautomates->currentText()[21].digitValue());;
    }

}
