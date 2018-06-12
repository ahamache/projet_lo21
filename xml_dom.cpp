#include "Xml_Dom.h"
#include <iostream>
using namespace std;

Xml_Dom::Xml_Dom() : QWidget()
{

    QDomDocument *dom = new QDomDocument("list_Automate"); // Création de l'objet DOM
    QFile xml_doc("save_autocell.xml");// On ouvre le fichier XML qui contient les automates

    if(!xml_doc.open(QIODevice::ReadOnly))// Si l'on n'arrive pas à ouvrir le fichier XML.
    {
        QMessageBox::warning(this,"Erreur à l'ouverture du document XML","Le document XML n'a pas pu être ouvert. Vérifiez que le nom est le bon et que le document est bien placé");
        return;
    }

    if (!dom->setContent(&xml_doc)) // On associe le fichier XML à l'objet DOM, si ça ne fonctionne pas alors fermeture du document XML avec message d'erreur
    {
        xml_doc.close();
        QMessageBox::warning(this,"Erreur à l'ouverture du document XML","Le document XML n'a pas pu être attribué à l'objet QDomDocument.");
        return;
    }
    xml_doc.close(); //on ferme le fichier xml

    QDomElement dom_element=dom.documentElement(); //dom_element a pour valeur tout le document XML
    QDomNode noeud = dom_element.firstChildElement(); //noeud a pour valeur le premier noeud du fichier -->nous va être <automate1D>


    ////////// dans le fichier il y a les trois types d'automates stockés

    while(!noeud.isNull()){ //parcours de tous les automates

        if(noeud.tagName() == "Automate1D"){

            QDomNode n2 = noeud.firstChildElement(); //on regarde à l'intérieur de Automate1D

            if(n2.tagName() == "Numero"){ //on vérifie que les balises sont correctes
                num=n2.text(); //on récupère le numéro de la règle écrite à l'intérieur
                AutomateManager::getInstance().getAutomate1D(num); //besoin que d'un des deux paramètres
                }

            else if(n2.tagName() == "NumeroBit"){
                numBit=n2.text;
                AutomateManager::getInstance().getAutomate1D(numBit); //besoin que d'un des deux paramètres
                }

            //inutile de chercher la deuxième balise car on a besoin que d'un des deux attributs pour le constructeur

            else QMessageBox::warning(this,"Erreur de lecture du document XML","Le document XML présente une balise incorrecte.");

            noeud=noeud.nextSibling(); //on passe au noeud automate suivant
        }

        else if(noeud.tagName() == "Automate2D"){

            QDomNode n2 = noeud.firstChildElement();
            int t=1;

            while(!n2.isNull() && t==1){

                if(n2.tagName() == "Minimum vivant")
                    miniV=n2.text();

                else if(n2.tagName() == "Maximum vivant")
                    maxiV=n2.text();

                else if(n2.tagName() == "Minimum mort")
                    miniM=n2.text();

                else if(n2.tagName() == "Maximum mort")
                    maxiM=n2.text();

                else t=0; //vérification de la validité des balises

                n2=n2.nextSibling();
            }

            if(t==1)
                AutomateManager::getInstance().getAutomate2D(miniV, maxiV, miniM, maxiM);
            noeud=noeud.nextSibling();
        }

        else if(noeud.tagName() == "Automate Epidemie"){

            QDomNode n2 = noeud.firstChildElement();
            int t=1;

            while(!n2.isNull() && t==1){

                if(n2.tagName() == "Chance 1")
                    c1=n2.text();

                else if(n2.tagName() == "Chance 2")
                    c2=n2.text();

                else t=0;

                n2=n2.nextSibling();
            }
            if (t==1)
                AutomateManager::getInstance().getAutomateEp(c1, c2);
            noeud=noeud.nextSibling();
        } //vérifier que les deux attributs sont bien remplsi !!!

}
}

Xml_Dom::ajouter_Automate1D(unsigned int n){ //ajouter avec le numero de règle

    QDomDocument dom("list_Automate");
    QFile doc_xml("save_autocell.xml"); //on charge le document existant

    if(!doc_xml.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
    return;}

    if(!dom.setContent(&doc_xml)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
        return;}

    doc_xml.close();

    QDomElement docElem = dom.documentElement(); //permet d'explorer le nouveau document DOM

    QDomElement aut_elem = dom.createElement("Automate 1D"); // On crée un QDomElement qui a comme nom de balise "Automate 1D".

    QDomElement num_elem = aut_elem.createElement("Numero"); //on créé un nouvel élément dont le nom de balise est Numero
    QDomText num_text = num_elem.createTexteNode(n); //à l'intérieur de la balise on ajoute le texte n

    QDomElement numB_elem = aut_elem.createElement("Numero Bit");
    QDomText numB_text = numB_elem.createTexteNode(NumToNumBit(n));

    docElem.appendChild(aut_elem); //on ajoute le noeud racine au DOM
    aut_elem.appendChild(num_elem); //on rajoute ses deux noeuds fils
    aut_elem.appendChild(numB_elem);

    QString write_doc = dom.toString(); //on transforme DOM en string

    QFile fichier("save_autocell.xml"); //on créer un QFile pour pouvoir écrire dedans
    if(!fichier.open(QIODevice::WriteOnly)){ //on ouvre le fichier
        fichier.close();
        QMessageBox::critical(this,"Erreur","Impossible d'écrire dans le document XML");
        return;
    }

    QTextStream stream(&fichier);
    stream << "<?xml version='1.0' encoding='UTF-8'?>"<<write_doc; // l'opérateur << permet d'écrire dans write_doc
    fichier.close();
}

Xml_Dom::ajouter_Automate2D(unsigned int mnV, unsigned int mxV, unsigned int mnM, unsigned int mxM){ //ajouter avec le numero de règle

  QDomDocument dom("list_Automate");
    QFile doc_xml("save_autocell.xml"); //on charge le document existant

    if(!doc_xml.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
    return;}

    if(!dom.setContent(&doc_xml)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
        return;}

    doc_xml.close();

    QDomElement docElem = dom.documentElement();

    QDomElement aut_elem = dom.createElement("Automate 2D");

    QDomElement mnV_elem = aut_elem.createElement("Minimum vivant");
    QDomText mnV_text = mnV_elem.createTextNode(mnV);

    QDomElement mxV_elem = aut_elem.createElement("Maximum vivant");
    QDomText mxV_text = mxV_elem.createTextNode(mxV);

    QDomElement mnM_elem = aut_elem.createElement("Minimum mort");
    QDomText mnM_text = mnM_elem.createTextNode(mnM);

    QDomElement mxM_elem = aut_elem.createElement("Maximum mort");
    QDomText mxM_text = mxM_elem.createTextNode(mxM);

    docElem.appendChild(aut_elem);
    aut_elem.appendChild(mnV_elem); //on rajoute les quatre noeuds fils
    aut_elem.appendChild(mxV_elem);
    aut_elem.appendChild(mnM_elem);
    aut_elem.appendChild(mxM_elem);

    QString write_doc = dom.toString(); //on transforme DOM en string

    QFile fichier("save_autocell.xml"); //on créer un QFile pour pouvoir écrire dedans
    if(!fichier.open(QIODevice::WriteOnly)){ //on ouvre le fichier
        fichier.close();
        QMessageBox::critical(this,"Erreur","Impossible d'écrire dans le document XML");
        return;
    }

    QTextStream stream(&fichier);
    stream << "<?xml version='1.0' encoding='UTF-8'?>"<< write_doc; // l'opérateur << permet d'écrire dans write_doc
    fichier.close();

}

Xml_Dom::ajouter_AutomateEp(unsigned int c1, unsigned int c2){ //ajouter avec le numero de règle

    QDomDocument dom("list_Automate");
    QFile doc_xml("save_autocell.xml"); //on charge le document existant

    if(!doc_xml.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
    return;}

    if(!dom.setContent(&doc_xml)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
        return;}

    doc_xml.close();

    QDomElement docElem = dom.documentElement();

    QDomElement aut_elem = dom.createElement("Automate Epidemie");

    QDomElement c1_elem = aut_elem.createElement("Chance 1");
    QDomText c1_text = c1_elem.createTextNode(mnV);

    QDomElement c2_elem = aut_elem.createElement("Chance 2");
    QDomText c2_text = c2_elem.createTextNode(mxV);

    docElem.appendChild(aut_elem);
    aut_elem.appendChild(c1_elem); //on rajoute les quatre noeuds fils
    aut_elem.appendChild(c2_elem);

    QString write_doc = dom.toString(); //on transforme DOM en string

    QFile fichier("save_autocell.xml"); //on créer un QFile pour pouvoir écrire dedans
    if(!fichier.open(QIODevice::WriteOnly)){ //on ouvre le fichier
        fichier.close();
        QMessageBox::critical(this,"Erreur","Impossible d'écrire dans le document XML");
        return;
    }

    QTextStream stream(&fichier);
    stream << "<?xml version='1.0' encoding='UTF-8'?>"<< write_doc; // l'opérateur << permet d'écrire dans write_doc
    fichier.close();

}



Xml_Dom::~Xml_Dom()
{

}
