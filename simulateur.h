#ifndef SIMULATEUR_H_INCLUDED
#define SIMULATEUR_H_INCLUDED

//#include "etat.h"
#include "automate.h"

using namespace std;
/*!
 * \brief The Simulateur class
 * Le simulateur permet de créer de nouvelles générations en appliquant les règles de transition d'un automate en partant d'un état de départ
 */
class Simulateur{
    /*!
     * \brief automate
     * Le simulateur simule de nouvelles générations à partir des règles de l'automate qu'on lui attribut
     */
    const Automate& automate;
    /*!
     * \brief depart
     * Le simulateur a besoin d'un état de départ à partir duquel il va générer les générations futures en prenant en compte l'état des cellules qui le compose
     */
    const Etat* depart;
    /*!
     * \brief etats
     * etats est un tableau contenant l'ensemble des nbMaxEtats derniers états qui ont été générés par le simulateur
     * Ces états sont stockés dans le tableau etats sous la forme d'une File
     */
    Etat** etats;
    /*!
     * \brief nbMaxEtats
     * nbMaxEtats est le nombre maximum d'états que le simulateur garde en mémoire
     */
    unsigned int nbMaxEtats; //nb max qu'on sauvegarde dans le tableau etats
    /*!
     * \brief rang
     * L'attribut rang permet de sauvegarder l'indice du dernier état généré stocké dans le tableau etats
     */
    unsigned int rang;
    /*!
     * \brief build
     * La fonction build supprime l'objet contenu dans le tableau etats à l'indice cellule rentré en paramètre et en construit un nouveau à la place
     * \param c : indice où va être rentré l'objet dans le tableau etats
     */
    void build(unsigned int c);
    /*!
     * \brief Simulateur
     * On ne souhaite pas autoriser la recopie donc on définit l'opérateur à delete
     * \param s : prend en paramètre un simulateur pour pouvoir le recopier dans la nouvelle instance
     */
    Simulateur(const Simulateur& s)=delete;
    /*!
     * \brief operator =
     * De même, on ne souhaite pas avoir d'opérateur d'affectation
     * \param s : prend en paramètre un simulateur pour pouvoir l'affecter à un nouvel automate
     * \return retourne une nouvelle instance d'automate recopiée de l'automate en paramètre
     */
    Simulateur& operator=(const Simulateur& s)=delete;
public :
    /*!
     * \brief Simulateur
     * On définit deux constructeurs différents
     * \param a : prend en paramètre un automate pour pouvoir initialiser l'automate dont on va simuler des générations
     * \param buf : permet d'initialiser le nombre d'état maximum que stocke le tableau etats
     */
    Simulateur(const Automate& a, unsigned int buf=2);
    /*!
     * \brief Simulateur
     * \param a : correspond à l'automate que l'on va simuler
     * \param dep : est l'état de départ qui permet de débuter la simulation
     * \param buffer : permet d'initialiser le nombre d'état maximum que stocke le tableau etats
     */
    Simulateur(const Automate& a, const Etat& dep, unsigned int buffer=2);
    /*!
     * \brief setEtatDepart
     * setEtatDepart permet d'affecter au simulateur un état de départ rentré en paramètre de la fonction
     * \param e : est l'état de départ qui va permettre l'initialisation
     */
    void setEtatDepart(const Etat& e);
    /*!
     * \brief next
     * next permet de passer à la génération suivante
     */
    void next();
    /*!
     * \brief run
     *  run permet de créer nbSteps générations à la suite
     * \param nbSteps : nombre d'étapes que l'on veut générer
     */
    void run(unsigned int nbSteps);
    /*!
     * \brief reset
     * reset permet de réinitialiser le tableau etats
     */
    void reset();
    /*!
     * \brief dernier
     * \return : dernier élément généré et stocké dans le tableau etats
     */
    const Etat& dernier() const;
    /*!
     * \brief getRangDernier
     * \return : le rang du dernier élément généré et stocké dans le tableau etats
     */
    unsigned int getRangDernier()const{return rang;} 
    ~Simulateur();

    /*!
     * \brief The Iterator class
     *
     * Permet de parcourir le tableau des états stockés par le simulateur
     */
    class Iterator {
        friend class Simulateur;
        /*!
         * \brief sim : pointeur vers le simulateur dont on parcourt les états stockés
         */
        Simulateur* sim;
        /*!
         * \brief i : indice du dernier élément généré et stocké par le simulateur
         */
        const unsigned int i;
        /*!
         * \brief j : indice de l'état sur lequel pointe le simulateur
         */
        int j;
        Iterator(Simulateur* s);
    public:
        Iterator();
        /*!
         * \brief isDone : renvoie 1 si l'itérateur a parcouru l'ensemble des états stockés par le simulateur
         * \return
         */
        bool isDone() const;
        Iterator& operator++();
        /*!
         * \brief operator *
         * \return l'état pointé par l'itérateur
         */
        Etat& operator* () const;
    };
    Iterator getIterator() {
        return Iterator(this);
    }

    /*!
     * \brief The ConstIterator class
     * * Permet de parcourir le tableau des états stockés par le simulateur sans pouvoir les modifier
     */
    class ConstIterator {
        friend class Simulateur;
        /*!
         * \brief sim : pointeur vers le simulateur dont on parcourt les états stockés
         */
        const Simulateur* sim;
        /*!
         * \brief i : indice du dernier élément généré et stocké par le simulateur
         */
        const unsigned int i;
        /*!
         * \brief j : indice de l'état sur lequel pointe le simulateur
         */
        int j;
        ConstIterator(Simulateur* s);
    public:
        ConstIterator();
        /*!
         * \brief isDone : renvoie 1 si l'itérateur a parcouru l'ensemble des états stockés par le simulateur
         * \return
         */
        bool isDone() const;
        ConstIterator& operator++();
        /*!
         * \brief operator *
         * \return l'état pointé par l'itérateur
         */
        Etat& operator* () const;
    };
    ConstIterator getConstIterator() {
        return ConstIterator(this);
    }
};


#endif // SIMULATEUR_H_INCLUDED
