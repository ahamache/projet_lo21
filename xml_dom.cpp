#include "xml_dom.h"
#include <iostream>
using namespace std;

Xml_Dom::Xml_Dom(const QString& param) : QWidget(){

    QDomDocument *dom = new QDomDocument("list_Automate"); /*! On créé un objet dom de type QDomDocument*/
    QFile xml_doc("save_autocell.xml"); /*! On ouvre le fichier XML qui contient les automates avec QFile*/

    if(!xml_doc.open(QIODevice::ReadWrite)) /*! On teste si on arrive à ouvrir le fichier XML */
    {
        QMessageBox::warning(this,"Erreur à l'ouverture du document XML","Le document XML n'a pas pu être ouvert. Vérifiez que le nom est le bon et que le document est bien placé");
        return;
    }

    if(xml_doc.size()==0){ /*! Si le fichier XML est vide, on insert l'en-tête du fichier XML et une balise racine Document puis on ferme le document*/
            xml_doc.write("<?xml version='1.0' encoding='UTF-8'?>\n<Document>\n</Document>");
            xml_doc.close();
            xml_doc.open(QIODevice::ReadOnly);
        }

    if (!dom->setContent(&xml_doc)) /*! On associe le fichier XML à l'objet DOM, si ça ne fonctionne pas alors fermeture du document XML avec message d'erreur*/
    {
        xml_doc.close();
        QMessageBox::warning(this,"Erreur à l'ouverture du document XML","Le document XML n'a pas pu être attribué à l'objet QDomDocument.");
        return;
    }
    xml_doc.close(); /*! On ferme le fichier xml après sa lecture */

    QDomElement dom_element=dom->documentElement(); /*! On affecte à dom_element la valeur du document XML entier*/
    QDomNode noeud = dom_element.firstChildElement(); /*! On affecte à noeud la valeur du premier noeud du fichier, <Automate1D> par exemple */


    /*! Dans le fichier on stocke les trois types d'automates */

    while(!noeud.isNull()){ /*! on rentre dans une boucle while pour parcourir tous les automates */

        QDomElement element = noeud.toElement(); /*! On transforme le noeud en élément */

        if(element.tagName()== param){

            if(element.tagName() == "Automate1D"){

                QDomNode n2 = noeud.firstChildElement(); /*! On regarde à l'intérieur de Automate1D */
                QDomElement elem2 = n2.toElement();

                if(elem2.tagName() == "Numero"){ /*! On vérifie que les balises sont correctes */

                    num=elem2.text().toInt(); /*! On récupère le numéro de la règle écrite à l'intérieur avec .toInt()*/
                    AutomateManager::getInstance().getAutomate1D(num); //besoin que d'un des deux paramètres
                    }

                else if(elem2.tagName() == "NumeroBit"){
                    numBit=elem2.text().toStdString();
                    AutomateManager::getInstance().getAutomate1D(numBit); //besoin que d'un des deux paramètres
                    }

                /*! Inutile de chercher la deuxième balise NumeroBit car on a besoin que d'un des deux attributs pour le constructeur*/

                else QMessageBox::warning(this,"Erreur de lecture du document XML","Le document XML présente une balise incorrecte.");
            }

            else if(element.tagName() == "Automate2D"){ /*! On effectue des opérations similaires pour Automate2D et AutomateEpidemie*/

                QDomNode n2 = noeud.firstChildElement();
                QDomElement elem2 = n2.toElement();

                int t=1;

                while(!n2.isNull() && t==1){

                    if(elem2.tagName() == "Minimumvivant")
                        miniV=elem2.text().toInt();

                    else if(elem2.tagName() == "Maximumvivant")
                        maxiV=elem2.text().toInt();

                    else if(elem2.tagName() == "Minimummort")
                        miniM=elem2.text().toInt();

                    else if(elem2.tagName() == "Maximummort")
                        maxiM=elem2.text().toInt();

                    else t=0; //vérification de la validité des balises

                    n2=n2.nextSibling();
                    elem2 = n2.toElement();
                }

                if(t==1)
                    AutomateManager::getInstance().getAutomate2D(miniV, maxiV, miniM, maxiM);
            }

            else if(element.tagName() == "AutomateEpidemie"){

                QDomNode n2 = noeud.firstChildElement();
                QDomElement elem2 = n2.toElement();

                int t=1;

                while(!n2.isNull() && t==1){
                    if(elem2.tagName() == "Chance1")
                        c1=elem2.text().toInt();

                    else if(elem2.tagName() == "Chance2")
                        c2=elem2.text().toInt();

                    else t=0;

                    n2=n2.nextSibling();
                    elem2 = n2.toElement();
                }
                if (t==1)
                    AutomateManager::getInstance().getAutomateEp(c1, c2);
        }

}
        noeud=noeud.nextSibling(); /*! On passe au noeud automate suivant avec nextSibling()*/

}

}


/*void Xml_Dom::ajouter_Automate(){

    QDomDocument dom("list_Automate");
    QFile doc_xml("save_autocell.xml"); //on charge le document existant

    if(!doc_xml.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
    return;}

    if(!dom.setContent(&doc_xml)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
        return;
    }

    doc_xml.close();

    QDomElement docElem = dom.documentElement(); //permet d'explorer le nouveau document DOM

     afficher : "quel type d'automate voulez-vous ajouter ?"
    for(std::vector<string>::iterator it = AutomateManager::getInstance().TypesAutomates.begin(); it != AutomateManager::getInstance().TypesAutomates.end(); ++it)
        afficher *it
    en fonction du choix de l'utiisateur, demander les paramètres et exécuter une des fonctions 1d, 2d ou ep avec ces paramètres
}*/

void Xml_Dom::ajouter_Automate1D(unsigned int n){ /*! On ajoute un nouvel automate dans le fichier XML en ajoutant son numéro de règle*/

    QDomDocument dom("list_Automate");
    QFile doc_xml("save_autocell.xml"); /*! On charge le document existant */

    if(!doc_xml.open(QIODevice::ReadWrite)){ /*! On teste l'ouverture du fichier */
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
    return;}

    if(!dom.setContent(&doc_xml)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML 22222");
        doc_xml.close();
        return;
    }

    doc_xml.close();

    QDomElement docElem = dom.documentElement(); /*! On créé un objet QDomElement qui contiendra l'ensemble des balises*/

    QDomElement aut_elem = dom.createElement("Automate1D"); /*! On crée un QDomElement qui a comme nom de balise "Automate 1D"*/

    QDomElement num_elem = dom.createElement("Numero"); /*! On créé un nouvel élément dont le nom de balise est Numero*/

    QString nb = QString::number(n); /*! On utilise un objet de type QString auquel on affecte le numéro de règle rentré en paramètre*/
    num_elem.appendChild(dom.createTextNode(nb)); /*! A l'intérieur de la balise on ajoute le texte n*/

    QDomElement numB_elem = dom.createElement("NumeroBit"); /*! On créé aussi une balise NumeroBit*/
    numB_elem.appendChild(dom.createTextNode(QString::fromStdString(NumToNumBit(n)))); /*! On rentre dans la balise NumeroBit la conversion en binaire du numéro de la règle*/

    docElem.appendChild(aut_elem); /*! On ajoute le noeud racine du nouvel automate à l'objet global DOM */
    aut_elem.appendChild(num_elem); /*! On attache au noeud racine ses deux noeuds fils */
    aut_elem.appendChild(numB_elem);

    QString write_doc = dom.toString(); /*! On transforme DOM en string */

    QFile fichier("save_autocell.xml"); /*! On créer un QFile pour pouvoir écrire dedans */
    if(!fichier.open(QIODevice::WriteOnly)){ /*! on ouvre le fichier*/
        fichier.close();
        QMessageBox::critical(this,"Erreur","Impossible d'écrire dans le document XML");
        return;
    }

    QTextStream stream(&fichier);
    stream << write_doc; /*! L'opérateur << permet d'écrire dans write_doc */
    fichier.close();
}

void Xml_Dom::ajouter_Automate2D(unsigned int mnV, unsigned int mxV, unsigned int mnM, unsigned int mxM){ /*! On veut ajouter un nouvel automate dans le fichier à partir des règles rentrées en argument*/

    if(mnV>mxV || mnM>mxM){
        QMessageBox::critical(this,"Erreur","Nombre de voisinnage invalide");
        return;
    }

    QDomDocument dom("list_Automate");
    QFile doc_xml("save_autocell.xml"); /*! On charge le document existant*/

    if(!doc_xml.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
    return;}

    if(!dom.setContent(&doc_xml)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
        return;
    }

    doc_xml.close();

    QDomElement docElem = dom.documentElement(); /*! Comme pour la fonction Ajouter_1D, on créé un noeud global dans lequel on créé de nouveaux noeuds afin de construire l'arborescence du XML*/

    QDomElement aut_elem = dom.createElement("Automate2D");

    QDomElement mnV_elem = dom.createElement("Minimumvivant"); /*! On rentre dans chaque balise les données qui leur correspondent rentrées en argument*/
    mnV_elem.appendChild(dom.createTextNode(QString::number(mnV)));

    QDomElement mxV_elem = dom.createElement("Maximumvivant");
    mxV_elem.appendChild(dom.createTextNode(QString::number(mxV)));

    QDomElement mnM_elem = dom.createElement("Minimummort");
    mnM_elem.appendChild(dom.createTextNode(QString::number(mnM)));

    QDomElement mxM_elem = dom.createElement("Maximummort");
    mxM_elem.appendChild(dom.createTextNode(QString::number(mxM)));

    docElem.appendChild(aut_elem);
    aut_elem.appendChild(mnV_elem); /*! On rattache les quatre noeuds fils au noeud racine*/
    aut_elem.appendChild(mxV_elem);
    aut_elem.appendChild(mnM_elem);
    aut_elem.appendChild(mxM_elem);

    QString write_doc = dom.toString(); /*! On transforme DOM en string */

    QFile fichier("save_autocell.xml"); /*! On créer un QFile pour pouvoir écrire dedans*/
    if(!fichier.open(QIODevice::WriteOnly)){ /*! On ouvre le fichier*/
        fichier.close();
        QMessageBox::critical(this,"Erreur","Impossible d'écrire dans le document XML");
        return;
    }

    QTextStream stream(&fichier);
    stream << write_doc; /*! L'opérateur << permet d'écrire dans write_doc */
    fichier.close();

}

void Xml_Dom::ajouter_AutomateEp(unsigned int c1, unsigned int c2){ /*! On ajoute un nouvel automate en le construisant à partir des paramètres avec lesquels la fonction est appelée*/

    QDomDocument dom("list_Automate");
    QFile doc_xml("save_autocell.xml"); /*! On charge le document existant */

    if(!doc_xml.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
    return;}

    if(!dom.setContent(&doc_xml)){
        QMessageBox::critical(this,"Erreur","Impossible d'ouvrir le ficher XML");
        doc_xml.close();
        return;
    }

    doc_xml.close();

    QDomElement docElem = dom.documentElement();

    QDomElement aut_elem = dom.createElement("AutomateEpidemie");

    QDomElement c1_elem = dom.createElement("Chance1");
    c1_elem.appendChild(dom.createTextNode(QString::number(c1)));

    QDomElement c2_elem = dom.createElement("Chance2");
    c2_elem.appendChild(dom.createTextNode(QString::number(c2)));

    docElem.appendChild(aut_elem);
    aut_elem.appendChild(c1_elem);
    aut_elem.appendChild(c2_elem);


    QString write_doc = dom.toString(); /*! on transforme DOM en string*/

    QFile fichier("save_autocell.xml"); /*! on créer un QFile pour pouvoir écrire dedans*/
    if(!fichier.open(QIODevice::WriteOnly)){ /*! on ouvre le fichier*/
        fichier.close();
        QMessageBox::critical(this,"Erreur","Impossible d'écrire dans le document XML");
        return;
    }

    QTextStream stream(&fichier);
    stream << write_doc; /*! L'opérateur << permet d'écrire dans write_doc */
    fichier.close();

}
