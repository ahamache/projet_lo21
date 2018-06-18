#include "principal.h"
#include "autocell.h"
#include "autocell2d.h"
#include "autocellepidemie.h"
#include "xml_dom.h"
#include<QApplication>
/*!
 * \brief fonction automain
 * \param Lié à QWidget
 */
Automain::Automain(QWidget *parent):QWidget(parent) {


c1=new QVBoxLayout;
/*! \brief Différents boutons */
label= new QLabel("Quel automate souhaitez vous générer ?");
a1d=new QPushButton("Automate 1 Dimension");
a2d=new QPushButton("Le jeu de la vie");
a3d=new QPushButton("L'automate Epidemie");
Quitter=new QPushButton("Quitter");

/*! \brief Gestion des tailles des grilles */
lon= new QSpinBox(this);
lon->setRange(0,25);
lon->setValue(10);
lonl=new QLabel("Taille");
lonc=new QVBoxLayout;
lonc->addWidget(lonl);
lonc->addWidget(lon);

lonoc=new QHBoxLayout;
lonoc->addLayout(lonc);


dlon= new QSpinBox(this);
dlon->setRange(0,25);
dlon->setValue(10);
dlonl=new QLabel("Longueur");
dlonc=new QVBoxLayout;
dlonc->addWidget(dlonl);
dlonc->addWidget(dlon);

dlonoc=new QHBoxLayout;
dlonoc->addLayout(dlonc);

clon= new QSpinBox(this);
clon->setRange(0,25);
clon->setValue(10);
clonl=new QLabel("Largeur");
clonc=new QVBoxLayout;
clonc->addWidget(clonl);
clonc->addWidget(clon);

clonoc=new QHBoxLayout;
clonoc->addLayout(clonc);



d3lon= new QSpinBox(this);
d3lon->setRange(0,50);
d3lon->setValue(25);
d3lonl=new QLabel("Longueur");
d3lonc=new QVBoxLayout;
d3lonc->addWidget(d3lonl);
d3lonc->addWidget(d3lon);

d3lonoc=new QHBoxLayout;
d3lonoc->addLayout(d3lonc);

c3lon= new QSpinBox(this);
c3lon->setRange(0,50);
c3lon->setValue(25);
c3lonl=new QLabel("Largeur");
c3lonc=new QVBoxLayout;
c3lonc->addWidget(c3lonl);
c3lonc->addWidget(c3lon);

c3lonoc=new QHBoxLayout;
c3lonoc->addLayout(c3lonc);

c1->addWidget(label);
c1->addWidget(a1d);
c1->addLayout(lonoc);
c1->addWidget(a2d);
c1->addLayout(dlonoc);
c1->addLayout(clonoc);
c1->addWidget(a3d);
c1->addLayout(d3lonoc);
c1->addLayout(c3lonoc);
c1->addWidget(Quitter);
setLayout(c1);
/*! \brief Gestion des clics sur les boutons */
connect(a1d,SIGNAL(clicked()),this,SLOT(lancerauto1d()));
connect(a2d,SIGNAL(clicked()),this,SLOT(lancerauto2d()));
connect(a3d,SIGNAL(clicked()),this,SLOT(lancerauto3()));

connect(Quitter,SIGNAL(clicked()),qApp,SLOT(quit()));
}
/*!
 * \brief Automain::lancerauto1d Appel de la fonction lors du clic sur lancer automate 1D, instancie un objet de la classe autocell
 */
void Automain::lancerauto1d(){
    unsigned int i=lon->value();
    AutoCell *fen = new AutoCell(nullptr,i);
    fen->show();
  }
/*!
 * \brief Automain::lancerauto2d Appel de la fonction lors du clic sur lancer automate 2D, instancie un objet de la classe autocell2d
 */
void Automain::lancerauto2d(){
    unsigned int i=dlon->value();
    unsigned int j=clon->value();
    AutoCell2d *fen = new AutoCell2d(nullptr,i,j);
    fen->show();
  }

/*!
 * \brief Automain::lancerauto3 Appel de la fonction lors du clic sur lancer automate Epidemie, instancie un objet de la classe autocellEpidemie
 */
void Automain::lancerauto3(){
    unsigned int i=d3lon->value();
    unsigned int j=c3lon->value();
    AutoCellepidemie *fen = new AutoCellepidemie(nullptr,i,j);
    fen->show();
  }
