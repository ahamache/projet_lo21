#include "simulateur.h"

void Simulateur::setEtatDepart(const Etat& e){
    if((typeid(automate).name()=="Automate1D" && e.getLongueur()!=1) || (typeid(automate).name()=="Automate2D" && e.getLongueur()==1))
        throw AutomateException("erreur : dimensions de l'automate et de l'etat incompatibles");
    depart = &e;
    if(etats[0]==nullptr)
        etats[0]=new Etat(e);
    else *etats[0]=e;
    reset();
}

Simulateur::Simulateur(const Automate& a, unsigned int buf): automate(a), depart(nullptr), etats(nullptr), nbMaxEtats(buf),rang(0) {
    etats = new Etat*[nbMaxEtats];
	for (unsigned int i = 0; i < nbMaxEtats; i++)
        etats[i] = nullptr;
}

Simulateur::Simulateur(const Automate& a, const Etat& dep, unsigned int buffer) : automate(a), etats(nullptr), nbMaxEtats(buffer), rang(0){
    etats = new Etat*[nbMaxEtats];
	for (unsigned int i = 0; i < nbMaxEtats; i++)
        etats[i] = nullptr;
	setEtatDepart(dep);
}


void Simulateur::build(unsigned int cellule) {
	if (cellule >= nbMaxEtats)
        throw AutomateException("erreur taille buffer");
	/*if (etats[cellule] == nullptr)
        etats[cellule] = new Etat;*/
	delete etats[cellule];
	etats[cellule] = new Etat;
}

void Simulateur::reset(){
    if (depart==nullptr) throw AutomateException("etat depart indefini");
	build(0); *etats[0] = *depart;
	rang = 0;
}


void Simulateur::next() {
	if (depart == nullptr)
        throw AutomateException("etat depart indefini");
	rang++;
	build(rang%nbMaxEtats);
	automate.AppliquerTransition(*etats[(rang - 1) % nbMaxEtats], *etats[rang%nbMaxEtats]);
	//std::cout << "stockage a l'indice " << rang%nbMaxEtats <<"; rang = "<<rang<< "\n";
}

void Simulateur::run(unsigned int nb_steps) {
	for (unsigned int i = 0; i < nb_steps; i++) next();
}

const Etat& Simulateur::dernier() const {
	return *etats[rang%nbMaxEtats];
}

Simulateur::~Simulateur() {
	for (unsigned int i = 0; i < nbMaxEtats; i++) delete etats[i];
	delete[] etats;
}

Simulateur::Iterator::Iterator(Simulateur* s) :sim(s), i((s->rang) % (s->nbMaxEtats)) {
	if (s->rang < sim->nbMaxEtats) j = 0; else j = i + 1;
	/*std::cout << "rang = "<< sim->rang << "\n";
	std::cout << "initialisation de l'itérateur : j= " << j << "\n\n";*/
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
	//if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
	return *this;
}
Etat& Simulateur::Iterator::operator* () const {
	if (isDone())
		throw AutomateException("error, indirection on an iterator which is done");
	//std::cout << "j = " << j << "\n" << "affichage de l'indice numero " << j % (sim->nbMaxEtats) << "\n";
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
	//if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
	return *this;
}

Etat& Simulateur::ConstIterator::operator* () const {
	if (isDone())
		throw AutomateException("error, indirection on an iterator which is done");
	return *sim->etats[j%sim->nbMaxEtats];
}