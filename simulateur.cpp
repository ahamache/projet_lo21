#include "simulateur.h"

void Simulateur::setEtatDepart(const Etat& e){
    if((typeid(automate).name()=="Automate1D" && e.getLongueur()!=1) || (typeid(automate).name()=="Automate2D" && e.getLongueur()==1))
        throw AutomateException("erreur : dimensions de l'automate et de l'etat incompatibles");
    depart = &e; /*! L'attribut depart est initialisé avec l'etat rentré en paramètre de la fonction*/
    if(etats[0]==nullptr)
        etats[0]=new Etat(e);
    else *etats[0]=e; /*! Le premier état stocké dans le tableau etats est l'état rentré en paramètres, si l'emplacement n'a pas été créé alors il l'est */
    reset();
}

Simulateur::Simulateur(const Automate& a, unsigned int buf): automate(a), depart(nullptr), etats(nullptr), nbMaxEtats(buf),rang(0) {
    etats = new Etat*[nbMaxEtats]; /*! On construit le tableau etats avec la taille rentrée en paramètre nbMaxEtats*/
    for (unsigned int i = 0; i < nbMaxEtats; i++)
        etats[i] = nullptr; /*! On initialise chaque emplacement du tableau à nullptr*/
}

Simulateur::Simulateur(const Automate& a, const Etat& dep, unsigned int buffer) : automate(a), etats(nullptr), nbMaxEtats(buffer), rang(0){
    etats = new Etat*[nbMaxEtats];/*! On construit le tableau etats avec la taille rentrée en paramètre nbMaxEtats*/
    for (unsigned int i = 0; i < nbMaxEtats; i++)
        etats[i] = nullptr;/*! On initialise chaque emplacement du tableau à nullptr*/
    setEtatDepart(dep); /*! On initialise l'attribut départ avec l'etat rentré en paramètre*/
}


void Simulateur::build(unsigned int cellule) {
    if (cellule >= nbMaxEtats)
        throw AutomateException("erreur taille buffer");
    /*if (etats[cellule] == nullptr)
        etats[cellule] = new Etat;*/
    delete etats[cellule]; /*! On détruit la cellule existante située dans le tableau à l'indice donné en paramètre puis on en créé un nouveau*/
    etats[cellule] = new Etat;
}

void Simulateur::reset(){
    if (depart==nullptr) throw AutomateException("etat depart indefini");
    build(0); /*! on réinitialise l'état stocké dans le premier emplacement du tableau etats*/
    *etats[0] = *depart; /*! Le premier état stocké du tableau etats redevient l'état de départ avec lequel le simulateur a été initialisé*/
    rang = 0; /*! L'indice du dernier état généré redevient 0*/
}


void Simulateur::next() {
    if (depart == nullptr)
        throw AutomateException("etat depart indefini");
    rang++; /*! Comme on génère un nouvel état on augmente l'indice du dernier état généré de 1*/
    build(rang%nbMaxEtats); /*! On stocke le nouvel état dans le tableau etats et en remplaçant éventuellement celui qui était stocké à cet emplacement avant*/
    automate.AppliquerTransition(*etats[(rang - 1) % nbMaxEtats], *etats[rang%nbMaxEtats]); /*! On applique la transition depuis l'état à la génération t à l'état de la génération t+1*/
}

void Simulateur::run(unsigned int nb_steps) {
    for (unsigned int i = 0; i < nb_steps; i++) next(); /*! On génère le nombre de fois voulues des nouvelles étapes de l'évolution de la population*/
}

const Etat& Simulateur::dernier() const {
    return *etats[rang%nbMaxEtats]; /*! On retourne l'état qui a été généré en dernier grâce à l'attribut rang*/
}

Simulateur::~Simulateur() {
    for (unsigned int i = 0; i < nbMaxEtats; i++) delete etats[i];
    delete[] etats; /*! On détruit l'ensemble des états stockés dans etats puis le tableau lui-même*/
}

Simulateur::Iterator::Iterator(Simulateur* s) :sim(s), i((s->rang) % (s->nbMaxEtats)) {
    if (s->rang < sim->nbMaxEtats) j = 0; else j = i + 1;
}

Simulateur::Iterator::Iterator() : sim(nullptr), i(0), j(0) {}

bool Simulateur::Iterator::isDone() const {
    return ((sim == nullptr) ||
        (j == i + 1 && sim->rang<sim->nbMaxEtats) ||
        (j == i + 1 + sim->nbMaxEtats && sim->rang >= sim->nbMaxEtats));
}

Simulateur::Iterator& Simulateur::Iterator::operator++() {
    if (isDone())
        throw AutomateException("error, next on an iterator which is done");
    j = (j + 1);
    return *this;
}
Etat& Simulateur::Iterator::operator* () const {
    if (isDone())
        throw AutomateException("error, indirection on an iterator which is done");
    return *sim->etats[j % (sim->nbMaxEtats)];
}



Simulateur::ConstIterator::ConstIterator(Simulateur* s) :sim(s), i((s->rang) % (s->nbMaxEtats)) {
    if (s->rang < sim->nbMaxEtats) j = 0; else j = i + 1;
}

Simulateur::ConstIterator::ConstIterator() :sim(nullptr), i(0), j(0) {}

bool Simulateur::ConstIterator::isDone() const {
    return ((sim == nullptr) ||
        (j == i + 1 && sim->rang<sim->nbMaxEtats) ||
        (j == i + 1 + sim->nbMaxEtats && sim->rang >= sim->nbMaxEtats));
}

Simulateur::ConstIterator& Simulateur::ConstIterator::operator++() {
    if (isDone())
        throw AutomateException("error, next on a constiterator which is done");
    j = (j + 1);
    return *this;
}

Etat& Simulateur::ConstIterator::operator* () const {
    if (isDone())
        throw AutomateException("error, indirection on an iterator which is done");
    return *sim->etats[j%sim->nbMaxEtats];
}
