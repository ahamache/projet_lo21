#include "autocell.h"

using namespace std;

short unsigned int NumBitToNum(const std::string& num) {
	if (num.size() != 8) throw AutomateException("Numero d'automate indefini");
	int puissance = 1;
	short unsigned int numero = 0;
	for (int i = 7; i >= 0; i--) {
		if (num[i] == '1') numero += puissance;
		else if (num[i] != '0') throw AutomateException("Numero d'automate indefini");
		puissance *= 2;
	}
	return numero;
}

std::string NumToNumBit(short unsigned int num) {
	std::string numeroBit;
	if (num > 256) throw AutomateException("Numero d'automate indefini");
	unsigned short int p = 128;
	int i = 7;
	while (i >= 0) {
		if (num >= p) {
			numeroBit.push_back('1');
			num -= p;
		}
		else { numeroBit.push_back('0'); }
		i--;
		p = p / 2;
	}
	return numeroBit;
}

Etat::Etat(unsigned int l, unsigned int c): nbColonne(c), nbLigne(l){

    if (c==0 || l==0)
        throw AutomateException("Mauvaise dimension de la matrice");

    else{
        valeurs = new bool*[nbLigne];
        for (unsigned int i=0; i<nbLigne; i++){
            valeurs[i] = new bool[nbColonne];
            for(unsigned int j=0; j<nbColonne; ++j)
                valeurs[i][j]=false;
                }
    }
}

void Etat::setCellule(unsigned int i, unsigned int j, bool val) {
	if (i >= nbLigne || j>=nbColonne)
        throw AutomateException("Cellule inexistante dimension trop élevée");
	valeurs[i][j] = val;
}

bool Etat::getCellule(unsigned int i, unsigned int j) const{
    if (i>=nbLigne|| j>=nbColonne)
        throw AutomateException("Cellule inexistante");
    return valeurs[i][j];
}

Etat::~Etat(){

for(unsigned int i=0; i<nbLigne;++i)
        delete valeurs[i];
    delete[] valeurs;
}

Etat::Etat(const Etat& e): nbLigne(e.nbLigne), nbColonne(e.nbColonne), valeurs(nullptr){

    if(nbLigne<=0 || nbColonne<=0)
        throw AutomateException("Mauvaise dimension");

    else {
        valeurs = new bool*[nbLigne];
        for(unsigned int i=0; i<nbLigne; ++i){

            valeurs[i]=new bool[nbColonne];
            for(unsigned int j=0; j<nbColonne; ++j){
                valeurs[i][j]=e.valeurs[i][j];
            }
        }
    }

}

Etat& Etat::operator=(const Etat& e){
    if(this != &e){ //auto-affectation impossible
        if(nbLigne!=e.nbLigne){ //on refait tout le tableau car pas bon nombre de ligne dès le début

            for(unsigned int i=0;i<nbLigne;++i)
                delete valeurs[i]; //on supprime chaque ligne
            delete[] valeurs;

            nbLigne=e.nbLigne;
            valeurs=new bool*[nbLigne];
            nbColonne=e.nbColonne;

            for(unsigned int i=0; i<nbLigne;++i)
                valeurs[i]=new bool[nbColonne];
            }

        else if(nbColonne!=e.nbColonne){

            nbColonne=e.nbColonne;
            for(unsigned int i=0;i<nbLigne;++i){
                delete valeurs[i];
                valeurs[i]=new bool[nbColonne];
            }
        }

        for (unsigned int i;i<nbLigne;i++){
            for(unsigned int j;j<nbColonne;++j)
                valeurs[i][j]=e.valeurs[i][j];
        }
    }

    return *this;
    }

std::ostream& operator<<(std::ostream& f, const Etat& e){

    for (unsigned int i=0; i<e.getLongueur(); ++i){
        for (unsigned int j=0; j<e.getLargeur(); ++j)
            f<<e.getCellule(i,j);
        f<<endl;
    }
    return f;

}

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
	if (etats[cellule] == nullptr)
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


