#include "autocell.h"
#include "simulateur.h"
#include "xml_dom.h"
#include <windows.h>
#include<QApplication>

bool btnContinuer = false;
bool btnReset = false;

/*!
 * \brief AutoCell::AutoCell constructeur de la page
 * \param parent héritage
 * \param i dimension états
 */
AutoCell::AutoCell(QWidget* parent,unsigned int i): AutoCellmere(parent), dimension(i) {
/*! charger automate depuis un fichier xml */
    page = new Xml_Dom(QString::fromStdString("Automate1D"));
    aff=new QLabel("Charger un automate déjà sauvegardé : ");
    lautomates = new QComboBox(this);
    testc=new QVBoxLayout;
    testc->addWidget(aff);
    testc->addWidget(lautomates);
    testoc=new QHBoxLayout;
    testoc->addLayout(testc);

    for(unsigned int i=0; i<255; i++){ //on inspecte l'ensemble du tableau pour voir si un automate existe déjà

        if(AutomateManager::getInstance().getElem1D(i)!=nullptr){ //si la regle a deja ete rentree dans le tableau d'automates1D[] d'AutomateManager
            QString s= QString::number(i);
            lautomates->addItem(s);
        }
    }

    b6 = new QPushButton("Charger l'automate");
    connect(b6,SIGNAL(clicked()),this,SLOT(setNum()));


    r=new QLabel("Choix de la règle de l'automate :");

    num= new QSpinBox(this);
    num->setRange(0,255);
    num->setValue(0);
    numl=new QLabel("Numéro");
    numc=new QVBoxLayout;
    numc->addWidget(numl);
    numc->addWidget(num);

    save=new QPushButton("Sauvegarder l'automate");

    numeroc=new QHBoxLayout;
    numeroc->addLayout(numc);

    list=new QLabel("Choix de l'initialisation:");
    liste = new QComboBox(this);
    liste->addItem("Vide");
    liste->addItem("Aleatoire");
    liste->addItem("Symetrie");
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
    b3=new QPushButton("Génereration Aléatoire(Uniquement si activé)");
    b4=new QPushButton("Vider la grille(Uniquement si activé)");
    b5=new QPushButton("Reset/Arret");

    zeroOneValidator=new QIntValidator(this);
    zeroOneValidator->setRange(0,1);

    Quitter=new QPushButton("Quitter");
/*! initialisation des bits pour le numero de règle*/
    for(unsigned int i=0; i<8; i++){
        numeroBit[i]=new QLineEdit(this);
        numeroBit[i]->setFixedWidth(20);
        numeroBit[i]->setMaxLength(1);
        numeroBit[i]->setText("0");
        numeroBit[i]->setValidator(zeroOneValidator);
        numeroBitl[i]= new QLabel;
        bitc[i]=new QVBoxLayout;
        bitc[i]->addWidget(numeroBitl[i]);
        bitc[i]->addWidget(numeroBit[i]);
        numeroc->addLayout(bitc[i]);
    }
    numeroBitl[0]->setText("111");
    numeroBitl[1]->setText("110");
    numeroBitl[2]->setText("101");
    numeroBitl[3]->setText("100");
    numeroBitl[4]->setText("011");
    numeroBitl[5]->setText("010");
    numeroBitl[6]->setText("001");
    numeroBitl[7]->setText("000");

    couche=new QVBoxLayout;
    couche->addLayout(testoc);
    couche->addWidget(b6);
    couche->addWidget(r);
    couche->addLayout(numeroc);
    couche->addWidget(save);
    couche->addLayout(listoc);
    couche->addLayout(clistoc);

    depart=new QTableWidget(1,dimension,this);
    couche->addWidget(depart);
    couche->addWidget(b3);
    couche->addWidget(b4);
    setLayout(couche);

/*! initialisation grille état de départ */
    unsigned int taille=25;
    depart->setFixedSize(dimension*taille,taille);
    depart->horizontalHeader()->setVisible(false);
    depart->verticalHeader()->setVisible(false);
    depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        for(unsigned int i=0; i<dimension;i++){
            depart->setColumnWidth(i,taille);
            depart->setItem(0,i,new QTableWidgetItem(""));
        }
/*! toutes les connections lors de l'appui d'un bouton*/
    connect(depart,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(cellActivation(QModelIndex)));
    connect(b4,SIGNAL(clicked()),this,SLOT(reset()));
    connect(b3,SIGNAL(clicked()),this,SLOT(genaleatoire()));
    connect(Quitter,SIGNAL(clicked()),qApp,SLOT(quit()));
    connect(save,SIGNAL(clicked()),this,SLOT(xml()));
    connect(num,SIGNAL(valueChanged(int)),this,SLOT(synchronizeNumToNumBit(int)));
    connect(b2,SIGNAL(clicked()),this,SLOT(b2_clicked()));
    connect(b5,SIGNAL(clicked()),this,SLOT(b5_clicked()));
    connect(b5,SIGNAL(clicked()),this,SLOT(reset2()));
    for(unsigned int i=0;i<8;i++)
        connect(numeroBit[i],SIGNAL(textChanged(QString)),this,SLOT(synchronizeNumBitToNum()));

/*! genere la grille pour les états qui seront remplis au cours de la simulation*/
    simulation=new QPushButton("Simulation");
    couche->addWidget(simulation);
    connect(simulation,SIGNAL(clicked()),this,SLOT(faireSimulation()));
    etats=new QTableWidget(10,dimension,this);
    etats->setFixedSize(dimension*taille,10*taille);
    etats->horizontalHeader()->setVisible(false);
    etats->verticalHeader()->setVisible(false);
    etats->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    etats->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //non editable
    etats->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(unsigned int i=0; i<dimension;i++){
        etats->setColumnWidth(i,taille);
        etats->setRowHeight(i,taille);
        for(unsigned int j=0; j<10;j++){
            etats->setItem(j,i,new QTableWidgetItem(""));
        }
    }
    couche->addWidget(etats);
    couche->addWidget(b5);
    couche->addWidget(b2);
    couche->addWidget(Quitter);

}
/*!
 * \brief AutoCell::faireSimulation réalise la simulation d'un automate
 */
void AutoCell::faireSimulation(){
    ::btnReset = false;
    /*!
     * \brief e creation d'un état
     */
    Etat e(1,dimension);
    /*! on définit les cellules de l'etat*/
    for(unsigned int i=0; i<dimension;i++)
        if(depart->item(0,i)->text()!="_")e.setCellule(0,i,1);
    /*!
     * \brief A création d'un automate
     */
    const Automate1D& A= AutomateManager::getInstance().getAutomate1D(num->value());
    /*!
     * \brief S création d'un simulateur
     */
    Simulateur S(A,e);
    /*! mode pas à pas */
    if(cliste->currentIndex()==0){
        for(unsigned int i=0; i<10; i++){
            /*! génération de tous les successeurs*/

            S.next();
            /*! on créé le dernier état généré*/
            const Etat& d=S.dernier();
            for(unsigned int j=0; j<dimension; j++){
                if(d.getCellule(0,j)==1){
                    etats->item(i,j)->setBackgroundColor("black");
                }
                else{
                    etats->item(i,j)->setBackgroundColor("white");

                }
            }
            /*! attente de l'appui sur le bouton continuer simulation */
               while ( ::btnContinuer != true ){
                    QThread::msleep(20);
                   QCoreApplication::processEvents();
                   if(::btnReset == true){
                    return;
                   }

            }
      ::btnContinuer = false;
        }


    }
    /*! Mode automatique avec pauses*/
    else{
    for(unsigned int i=0; i<10; i++){
        S.next();
        if(cliste->currentIndex()==1){
            if(::btnReset == true){
             return;
            }
            /*! temps de pause*/
            unsigned int a=clisteNor->value();
             QThread::msleep(a*200);
             QCoreApplication::processEvents();

        }
        const Etat& d=S.dernier();
        for(unsigned int j=0; j<dimension; j++){
            if(d.getCellule(0,j)==1){
                etats->item(i,j)->setBackgroundColor("black");
            }
            else{
                etats->item(i,j)->setBackgroundColor("white");

            }
        }
      }
   }
}
/*!
 * \brief AutoCell::b2_clicked changement du bouton pour continuer simulation
 */
void AutoCell::b2_clicked(){
    ::btnContinuer = true;
}

/*!
 * \brief AutoCell::genaleatoire generation aléatoire
 */
void AutoCell::genaleatoire(){
    if(liste->currentIndex()==1){
        for(unsigned int i=0; i<dimension;i++){
            /*! probabilité 1/2 d'avoir un état ou l'autre*/
            unsigned int a=rand()%2;
            if(a==0){
                depart->item(0,i)->setText("");
                depart->item(0,i)->setBackgroundColor("white");
                depart->item(0,i)->setTextColor("white");
            }
            else{
                depart->item(0,i)->setText("_");
                depart->item(0,i)->setBackgroundColor("black");
                depart->item(0,i)->setTextColor("black");
            }
        }
   }
}

/*!
 * \brief AutoCell::reset reset de la grille si mode activé
 */
void AutoCell::reset(){
    if(liste->currentIndex()==0){
        for(unsigned int i=0; i<dimension;i++){
            depart->item(0,i)->setText("");
            depart->item(0,i)->setBackgroundColor("white");
            depart->item(0,i)->setTextColor("white");
        }
    }
}
/*!
 * \brief AutoCell::reset2 reset lors de l'arrêt
 */
void AutoCell::reset2(){
        for(unsigned int i=0; i<dimension;i++){
            depart->item(0,i)->setText("");
            depart->item(0,i)->setBackgroundColor("white");
            depart->item(0,i)->setTextColor("white");
        }
    }


/*!
 * \brief AutoCell::b5_clicked réinitialisation grille de simulation
 */
void AutoCell::b5_clicked(){
    unsigned int taille=25;
    for(unsigned int i=0; i<dimension;i++){
        etats->setColumnWidth(i,taille);
        etats->setRowHeight(i,taille);
        for(unsigned int j=0; j<10;j++){
            etats->setItem(j,i,new QTableWidgetItem(""));
        }
    }
    ::btnReset = true;
}


/*!
 * \brief AutoCell::cellActivation change l'etat d'une cellule lorsque l'on clique dessus
 */
void AutoCell::cellActivation(const QModelIndex& index){
    if (depart->item(0,index.column())->text()==""){//desactivé
        depart->item(0,index.column())->setText("_");
        depart->item(0,index.column())->setBackgroundColor("black");
        depart->item(0,index.column())->setTextColor("black");
        if(liste->currentIndex()==2){
            depart->item(0,dimension-1-index.column())->setText("_");
            depart->item(0,dimension-1-index.column())->setBackgroundColor("black");
            depart->item(0,dimension-1-index.column())->setTextColor("black");

        }
    }
    else{ //activé
        depart->item(0,index.column())->setText("");
        depart->item(0,index.column())->setBackgroundColor("white");
        depart->item(0,index.column())->setTextColor("white");
        if(liste->currentIndex()==2){
            depart->item(0,dimension-1-index.column())->setText("");
            depart->item(0,dimension-1-index.column())->setBackgroundColor("white");
            depart->item(0,dimension-1-index.column())->setTextColor("white");
        }
    }
}
/*!
 * \brief AutoCell::synchronizeNumToNumBit synchronisation d'un entier en bit
 */
void AutoCell::synchronizeNumToNumBit(int i){
    std::string numbit=NumToNumBit(i);
    for(unsigned int i=0;i<8;i++)
        numeroBit[i]->setText(QString(numbit[i]));
}
/*!
 * \brief AutoCell::synchronizeNumBitToNum synchronisation des bits en un entier
 */
void AutoCell::synchronizeNumBitToNum(){
    for (unsigned int i=0; i<8; i++)
        if(numeroBit[i]->text()=="")return;
    std::string str;
    for(unsigned int i=0;i<8;i++)
        str+=numeroBit[i]->text().toStdString();
    int i=NumBitToNum(str);
    num->setValue(i);

}


void AutoCell::xml(){
page->ajouter_Automate1D(num->value());
}

void AutoCell::setNum() {
    num->setValue(lautomates->currentText().toInt());
}
