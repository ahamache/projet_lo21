#ifndef AUTOMATE_H_INCLUDED
#define AUTOMATE_H_INCLUDED

#include <string>
#include <iostream>
#include<typeinfo>
#include "etat.h"
#include <cstdlib> //pour le rand()

using namespace std;
/*!
 * \brief The AutomateException class
 *
 * Gère les exceptions générées par des automates
 */
class AutomateException {
public:
AutomateException(const std::string& message):info(message) {}
std::string getInfo() const { return info; }
private:
std::string info;
};
/*!
 * \brief NumBitToNum
 * \param num : chaîne de caractères représentant un nombre en écriture binaire
 * \return Convertit la chaîne entrée en paramètre en un nombre de type unsigned int
 */
short unsigned int NumBitToNum(const std::string& num);
/*!
 * \brief NumToNumBit
 * \param num : entier
 * \return Convertit l'entier entré en paramètre en binaire et retourne le résultat au formt std::string
 */
std::string NumToNumBit(short unsigned int num);

/*!
 * \brief The Automate class
 *
 * Classe mère abstraite
 */
class Automate{
    /*!
     * \brief nbEtats
     *
     * Nombre d'états que peuvent prendre les cellules des grilles manipulées par cet automate
     */
    unsigned int nbEtats;
    friend class AutomateManager;
protected:
    /*!
     * \brief Constructeur de la classe Automate
     * \param n : nombre d'états que peuvent prendre les cellules
     */
    Automate(unsigned int n) : nbEtats(n){}
    /*!
     * \brief Constructeur par recopie de la classe Automate
     * \param a : automate à recopier
     */
    Automate(const Automate& a);
    /*!
     * \brief operator =
     * \return
     */
    Automate& operator=(const Automate&);
public:
    virtual ~Automate() {}
    /*!
     * \brief getNbEtats
     * \return le nombre d'états que peuvent prendre les cellules
     */
    unsigned int getNbEtats() const {return nbEtats;}
    /*!
     * \brief AppliquerTransition
     * Méthode virtuelle pure
     */
    virtual void AppliquerTransition(const Etat&, Etat&) const =0; //methode virtuelle pure --> la classe est abstraite donc non instanciable

};

/*!
 * \brief The Automate1D class
 */
class Automate1D : public Automate {
    /*!
     * \brief numero
     *
     * Numéro de règle de l'automate
     */
    unsigned short int numero;
    /*!
     * \brief numeroBit
     *
     * Numéro de règle de l'automate, au format binaire
     */
    std::string numeroBit;
    /*!
     * \brief Automate1D
     * \param num : numéro de règle de l'automate
     * \param nbE : nombre d'états que peuvent prendre les cellules
     */
    Automate1D(unsigned short int num, unsigned int nbE=2);
    /*!
     * \brief Automate1D
     * \param num : numéro de règle de l'automate, au format binaire
     * \param nbE : nombre d'états que peuvent prendre les cellules
     */
    Automate1D(const std::string& num, unsigned int nbE=2);
    friend class AutomateManager;
    /*!
     * \brief Constructeur de recopie de la Automate1D
     * \param a : Automate à recopier
     */
    Automate1D(const Automate1D& a);
    /*!
     * \brief operator =
     * \return
     */
    Automate1D& operator=(const Automate1D&);
public:
    /*!
     * \brief getNumero
     * \return le numéro de la règle au format unsigned int
     */
    unsigned short int getNumero() const { return numero; }
    /*!
     * \brief getNumeroBit
     * \return le numéro de la règle sous forme binaire au format std:string
     */
    const std::string& getNumeroBit() const { return numeroBit; }
    /*!
     * \brief AppliquerTransition
     *
     * Permet de générer l'état suivant en appliquant les règles de l'automate
     * \param dep : état de départ
     * \param dest : état généré
     */
    void AppliquerTransition(const Etat& dep, Etat& dest) const;

};

/*!
 * \brief operator << surchargé pour afficher un Automate1D
 * \param f : flux de sortie
 * \param t : automate à afficher
 */
std::ostream& operator<<(std::ostream& f, const Automate1D& t);


/*!
 * \brief The Automate2D class : automates implémentant le Jeu de la Vie
 */
class Automate2D : public Automate{
    /*!
     * \brief nbMinVivant
     *
     * sachant une cellule est vivante, c'est le nombre minimum de cellules vivantes dans son voisinage pour qu'elle survive
     */
    unsigned int nbMinVivant;
    /*!
     * \brief nbMaxVivant
     * sachant une cellule est vivante, c'est le nombre maximum de cellules vivantes dans son voisinage pour qu'elle survive
     */
    unsigned int nbMaxVivant;
    /*!
     * \brief nbMinMort
     * sachant une cellule est morte, c'est le nombre minimum de cellules vivantes dans son voisinage pour qu'elle renaisse
     */
    unsigned int nbMinMort;
    /*!
     * \brief nbMaxMort
     * * sachant une cellule est morte, c'est le nombre maximum de cellules vivantes dans son voisinage pour qu'elle renaisse
     */
    unsigned int nbMaxMort;
    /*!
     * \brief Automate2D
     * \param minV : correspond à nbMinV
     * \param maxV : correspond à nbMaxV
     * \param minM : correspond à nbMinM
     * \param maxM : correspond à nbMaxM
     */
    Automate2D(unsigned int minV=0, unsigned int maxV=0, unsigned int minM=0, unsigned int maxM=0);
    friend class AutomateManager;
    /*!
     * \brief Constructeur par recopie de la classe Automate2D
     * \param a : automate à recopier
     */
    Automate2D(const Automate2D& a);
    /*!
     * \brief operator = surchargé
     * \return
     */
    Automate2D& operator=(const Automate2D&);

public :
    /*!
     * \brief getMinV  retourne la valeur de l'attribut nbMinVivant
     * \return
     */
    unsigned int getMinV()const{return nbMinVivant;}
    /*!
     * \brief getMaxV   retourne la valeur de l'attribut nbMaxVivant
     * \return
     */
    unsigned int getMaxV()const{return nbMaxVivant;}
    /*!
     * \brief getMinM  retourne la valeur de l'attribut nbMinMort
     * \return
     */
    unsigned int getMinM()const{return nbMinMort;}
    /*!
     * \brief getMaxM retourne la valeur de l'attribut nbMaxMort
     * \return
     */
    unsigned int getMaxM()const{return nbMaxMort;}
    /*!
     * \brief CountVoisin : compte le nombre de voisins vivants dans le voisinage d'une cellule.
     * \param li : numéro de ligne de la cellule dont on veut compter le nombre de voisins vivants
     * \param co : numéro de colonne de la cellule dont on veut compter le nombre de voisins vivants
     * \param e : grille à laquelle appartient la cellule
     */
    unsigned int CountVoisin(unsigned int li, unsigned int co, const Etat& e) const;
    /*!
     * \brief AppliquerTransition
     *
     * Permet de générer l'état suivant en appliquant les règles de lautomate
     * \param dep : état de départ
     * \param dest : état généré
     */
    void AppliquerTransition(const Etat& dep, Etat& dest) const;
};

/*!
 * \brief operator << surchargé pour afficher un Automate2D
 * \param f : flux de sortie
 * \param A : automate à afficher
 */
std::ostream& operator<<(std::ostream& f, const Automate2D& A);


/*!
 * \brief The AutomateEpidemie class : classe implémentant la propagation d'une épidémie
 * Les cellules prennent 4 états :
 * 0 -> saine
 * 1 -> malade
 * 2 -> immunisée
 * 3 -> morte
 */
class AutomateEpidemie : public Automate {
    /*!
     * \brief chance1
     *
     * Probabilité sur 10 de mourir étant malade
     */
    unsigned int chance1;
    /*!
     * \brief chance2
     *
     * Probabilité sur 10 de tomber malade étant sain
     */
    unsigned int chance2;

    /*!
     * \brief Constructeur par recopie de la classe AutomateEpidemie
     * \param a
     */
    AutomateEpidemie(const AutomateEpidemie& a);
    /*!
     * \brief operator = surchargé pour permettre une affectation entre objets de la classe AutomateEpidemie
     * \return
     */
    AutomateEpidemie& operator=(const AutomateEpidemie&);

    friend class AutomateManager;

public :
    /*!
     * \brief Constructeur de la classe AutomateEpidemie
     * \param c1 : correspond à l'attribut chance1
     * \param c2 : correspond à l'attribut chance2
     */
    AutomateEpidemie(unsigned int c1 = 0, unsigned int c2 = 0);
    /*!
     * \brief getChance1
     * \return la valeur de l'attribut chance1
     */
    unsigned int getChance1() const { return chance1; }
    /*!
     * \brief getChance2
     * \return la valeur de l'attribut chance2
     */
    unsigned int getChance2() const { return chance2; }
    /*!
     * \brief CountVoisinMalade permet de savoir si la cellule a au moins un voisin malade dans son voisinage
     * \param li : numéro de ligne de la cellule
     * \param co : numéro de colonne de la cellule
     * \param e : grille à laquelle appartient la cellule
     * \return 1 s'il y a au moins un malade, 0 sinon
     */
    unsigned int CountVoisinMalade(unsigned int li, unsigned int co, const Etat& e) const;
    void AppliquerTransition(const Etat& dep, Etat& dest) const;

};

/*!
 * \brief operator << surchargé pour afficher un AutomateEpidemie
 * \param f : flux de sortie
 * \param A : automate à afficher
 */
std::ostream& operator<<(std::ostream& f, const AutomateEpidemie& A);

/*!
 * \brief The AutomateManager class
 * Gère l'ensemble des automates
 */
class AutomateManager{ //Le manager gere un ensemble d'automate qui peut etre 1D ou 2D
    /*!
     * \brief instance : instance unique de l'automate manager
     */
    static AutomateManager* instance;

    //Trois tableaux ou on stocke les differents automates :
    /*!
     * \brief automates1D : stocke les automates 1D générés
     */
    Automate1D* automates1D[256];
    /*!
     * \brief automates2D : stocke les automates 2D générés
     */
    Automate2D** automates2D;
    /*!
     * \brief automatesEp : stocke les automates Epidemie générés
     */
    AutomateEpidemie** automatesEp;

    //Attributs pour le tableau d'automates 2D
    /*!
     * \brief tailleTab2D : taille du tableau automates2D
     */
    unsigned int tailleTab2D;
    /*!
     * \brief nb2DStockes : nombre d'automates stockés dans le tableau automates2D
     */
    unsigned int nb2DStockes; //le rang du dernier automate 2D rentre

    //Attributs pour le tableau d'automates Epidemie
    /*!
     * \brief tailleTabEp : taille du tableau automatesEp
     */
    unsigned int tailleTabEp;
    /*!
     * \brief nbEpStockes : nombre d'automates stockés dans le tableau automatesEp
     */
    unsigned int nbEpStockes;
    /*!
     * \brief Constructeur de la classe AutomateManager
     */
    AutomateManager();
    ~AutomateManager();
    /*!
     * \brief Construction par recopie de la classe AutomateManager interdite
     */
    AutomateManager(const AutomateManager& A)=delete;
    /*!
     * \brief operator = surchargé : on interdit l'affectation entre objets de la classe AutomateManager
     */
    AutomateManager& operator=(const AutomateManager& A)=delete;//idem

    struct Handler{
        AutomateManager* instance;
        Handler(): instance(nullptr){}
        ~Handler(){
            delete instance;}
        };

public:
    static Handler handler;
    /*!
     * \brief getInstance
     * \return une instance de la classe AutomateManager
     */
    static AutomateManager& getInstance();
    static void libererInstance(){
        delete handler.instance;
        handler.instance=nullptr;
    }

    //fonctions pour recuperer un automate stocke par le simulateur ou le construire s'il n'existe pas
    /*!
     * \brief getAutomate1D retourne l'automate1D recherché, en l'ayant préalablement généré si l'automate n'avait pas été généré auparavant.
     * \param num : numéro de la règle de l'automate, au format short unsigned int
     * \return
     */
    const Automate1D& getAutomate1D(short unsigned int num);
    /*!
     * \brief getAutomate1D retourne l'automate1D recherché, en l'ayant préalablement généré si l'automate n'avait pas été généré auparavant.
     * \param num : numéro de la règle de l'automate sous forme binaire, au format string
     * \return
     */
    const Automate1D& getAutomate1D(const string& num);
    /*!
     * \brief getAutomate2D : retourne l'automate2D recherché, en l'ayant préalablement généré si l'automate n'avait pas été généré auparavant.
     * \param miniV : valeur de l'attribut nbMinVivant de l'automate recherché
     * \param maxiV : valeur de l'attribut nbMaxVivant de l'automate recherché
     * \param miniM : valeur de l'attribut nbMinMort de l'automate recherché
     * \param maxiM : valeur de l'attribut nbMaxMort de l'automate recherché
     */
    const Automate2D& getAutomate2D(unsigned int miniV, unsigned int maxiV, unsigned int miniM, unsigned int maxiM);
    /*!
     * \brief getAutomateEp : retourne l'automate2D recherché, en l'ayant préalablement généré si l'automate n'avait pas été généré auparavant.
     * \param c1 : valeur de l'attribut chance1 de l'automate recherché
     * \param c2 : valeur de l'attribut chance2 de l'automate recherché
     * \return
     */
    const AutomateEpidemie& getAutomateEp(unsigned int c1, unsigned int c2);

    /*!
     * \brief getElem1D : retourne l'élément du tableau automates1D à l'indice n
     */
    const Automate1D* getElem1D(unsigned int n) const; //permet de retourner l'element du tableau automates1D[]

    /*!
     * \brief getDim2D
     * \return la taille du tableau tailleTab2D
     */
    unsigned int getDim2D()const {return tailleTab2D;}
    /*!
     * \brief getNb2DStockes
     * \return le nombre d'automates 2D stockés dans le tableau automates2D
     */
    unsigned int getNb2DStockes()const {return nb2DStockes;}
    /*!
     * \brief indice_automate2D permet de rechercher l'indice d'un automate2D stocké par l'AutomateManager. L'automate est recherché selon les 4 attributs qui correspondent à sa règle
     * \param a : valeur de l'attribut nbMinVivant de l'automate2D que l'on recherche
     * \param b : valeur de l'attribut nbMaxVivant de l'automate2D que l'on recherche
     * \param c : valeur de l'attribut nbMinMort de l'automate2D que l'on recherche
     * \param d : valeur de l'attribut nbMaxMort de l'automate2D que l'on recherche
     * \return l'indice de l'automate cherché, ou -1 si l'automate n'a pas été trouvé
     */
    int indice_automate2D(unsigned int a, unsigned int b, unsigned int c, unsigned int d) const; //retourne l'indice dans le tableau ou -1 si jamais rentre
    /*!
     * \brief getElem2D renvoie l'automate2D stocké à la case d'indice n du tableau automates2D
     */
    const Automate2D* getElem2D(unsigned int n) const;

    /*!
     * \brief getDimEp
     * \return la taille du tableau tailleTabEp
     */
    unsigned int getDimEp()const { return tailleTabEp; }
    /*!
     * \brief getNbEpStockes
     * \return le nombre d'automates Epidemie stockés dans le tableau automatesEp
     */
    unsigned int getNbEpStockes()const { return nbEpStockes; }
    /*!
     * \brief indice_automateEp permet de rechercher l'indice d'un automateEpidemie stocké par l'AutomateManager. L'automate est recherché selon les 2 attributs qui correspondent à sa règle
     * \param c1 : valeur de l'attribut chance1 de l'automateEpidemie que l'on recherche
     * \param c2 : valeur de l'attribut chance2 de l'automateEpidemie que l'on recherche
     * \return l'indice de l'automate cherché, ou -1 si l'automate n'a pas été trouvé
     */
    int indice_automateEp(unsigned int c1, unsigned int c2) const;
    /*!
     * \brief getElemEp renvoie l'automateEpidemie stocké à la case d'indice n du tableau automatesEp
     */
    const AutomateEpidemie* getElemEp(unsigned int n) const;
};

#endif // AUTOMATE_H_INCLUDED
