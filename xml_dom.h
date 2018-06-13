//include dans le .pro : QT += xml

#ifndef XML_DOM_H_INCLUDED
#define XML_DOM_H_INCLUDED

#include <QtGui>
#include <QtXml>
#include <QWidget>

class Xml_Dom : public QWidget
{
    QFile xml_doc;
    QDomElement dom;
    QDomNode noeud;
    QDomElement element;
    unsigned int num;
    std::string numBit;
    unsigned int miniV;
    unsigned int maxiV;
    unsigned int miniM;
    unsigned int maxiM;
    unsigned int c1;
    unsigned int c2;
public:
    Xml_Dom();
	~Xml_Dom(){}
    ajouter_Automate();
	ajouter_Automate1D(unsigned int n);
    ajouter_Automate2D(unsigned int mnV, unsigned int mxV, unsigned int mnM, unsigned int mxM);
    ajouter_AutomateEp(unsigned int c1, unsigned int c2);
    unsigned int affiche1D();
    unsigned int affiche2D();
    unsigned int afficheEp();

};

#endif // XML_DOM_H_INCLUDED
