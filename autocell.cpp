#include "autocell.h"

using namespace std;

Etat1D::Etat1D(unsigned int n):nbColonnes(n),
	valeur(new bool [n]) {
	for (unsigned int i = 0; i < n; i++) valeur[i] = false;
}

void Etat1D::setCellule(unsigned int i, bool val) {
	if (i >= nbColonnes) throw AutomateException("Erreur Cellule");
	valeur[i] = val;
}

bool Etat1D::getCellule(unsigned int i) const {
	if (i >= nbColonnes) throw AutomateException("Erreur Cellule");
	return valeur[i];
}

Etat1D::Etat1D(const Etat1D& e):nbColonnes(e.nbColonnes),valeur(new bool[e.nbColonnes]){
	for (unsigned int i = 0; i < nbColonnes; i++) valeur[i] = e.valeur[i];
}

std::ostream& operator<<(std::ostream& f, const Etat1D& e) {
	for (unsigned int i = 0; i < e.getNbColonnes(); i++)
		if (e.getCellule(i)) f << char(178); else f << " ";
	return f;
}

Etat1D& Etat1D::operator=(const Etat1D& e) {
	if (this != &e) {
		if (nbColonnes != e.nbColonnes) {
			bool* newvaleur = new bool[e.nbColonnes];
			for (unsigned int i = 0; i < e.nbColonnes; i++) newvaleur[i] = e.valeur[i];
			bool* old = valeur;
			valeur = newvaleur;
			nbColonnes = e.nbColonnes;
			delete[] old;
		}else for (unsigned int i = 0; i < e.nbColonnes; i++) valeur[i] = e.valeur[i];
	}
	return *this;
}

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

Automate1D::Automate1D(unsigned short int num):numero(num),numeroBit(NumToNumBit(num)){
}

Automate1D::Automate1D(const std::string& num) :numero(NumBitToNum(num)),numeroBit(num) {
}



void Automate1D::appliquerTransition(const Etat1D& dep, Etat1D& dest) const {
	if (dep.getNbColonnes() != dest.getNbColonnes()) dest = dep;
	for (unsigned int i = 0; i < dep.getNbColonnes(); i++) {
		unsigned short int conf=0;
		if (i > 0) conf+=dep.getCellule(i - 1) * 4;
		conf+=dep.getCellule(i)*2;
		if (i < dep.getNbColonnes()-1) conf+=dep.getCellule(i + 1);
		dest.setCellule(i, numeroBit[7-conf]-'0');
	}
}

std::ostream& operator<<(std::ostream& f, const Automate1D& A) {
	f << A.getNumero() << " : " << A.getNumeroBit() << "\n";
	return f;
}

Simulateur1D::Simulateur1D(const Automate1D& a, unsigned int buffer): automate(a), etats(nullptr), depart(nullptr), nbMaxEtats(buffer),rang(0) {
	etats = new Etat1D*[nbMaxEtats];
	for (unsigned int i = 0; i < nbMaxEtats; i++) etats[i] = nullptr;
}

Simulateur1D::Simulateur1D(const Automate1D& a, const Etat1D& dep, unsigned int buffer):automate(a), etats(nullptr), depart(&dep), nbMaxEtats(buffer),rang(0) {
	etats = new Etat1D*[nbMaxEtats];
	for (unsigned int i = 0; i < nbMaxEtats; i++) etats[i] = nullptr;
	etats[0] = new Etat1D(dep);
}

void Simulateur1D::build(unsigned int cellule) {
	if (cellule >= nbMaxEtats) throw AutomateException("erreur taille buffer");
	if (etats[cellule] == nullptr) etats[cellule] = new Etat1D;
}

void Simulateur1D::setEtatDepart(const Etat1D& e) {
	depart = &e;
	reset();
}

void Simulateur1D::reset() {
	if (depart==nullptr) throw AutomateException("etat depart indefini");
	build(0); *etats[0] = *depart;
	rang = 0;
}

void Simulateur1D::next() {
	if (depart == nullptr) throw AutomateException("etat depart indefini");
	rang++;
	build(rang%nbMaxEtats);
	automate.appliquerTransition(*etats[(rang - 1) % nbMaxEtats], *etats[rang%nbMaxEtats]);
}

void Simulateur1D::run(unsigned int nb_steps) {
	for (unsigned int i = 0; i < nb_steps; i++) next();
}

const Etat1D& Simulateur1D::dernier() const {
	return *etats[rang%nbMaxEtats];
}

Simulateur1D::~Simulateur1D() {
	for (unsigned int i = 0; i < nbMaxEtats; i++) delete etats[i];
	delete[] etats;
}

Etat2D::Etat2D(unsigned int nbL, unsigned int nbC): nbLigne(nbL), nbColonne(nbC),valeurs(nullptr){

    if (nbC==0 || nbL==0) // && ou || ?
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

bool Etat2D::getCellule(unsigned int i, unsigned int j) const{
    if (i>=nbLigne|| j>=nbColonne)
        throw AutomateException("Cellule inexistante");
    return valeurs[i][j];
}

void Etat2D::setCellule(unsigned int i, unsigned int j, bool val){
    if (i>=nbLigne || j>=nbColonne)
        throw AutomateException("Cellule inexistante");
    valeurs[i][j]=val;
}

Etat2D::~Etat2D(){

    for(unsigned int i=0; i<nbLigne;++i)
        delete valeurs[i];
    delete[] valeurs;
}

/*void Etat2D::afficherGrille() const{

    cout<<"Grille 2D"<<endl;
    for (unsigned int i=0; i<nbLigne; i++){
        for (unsigned int j=0; j<nbColonne; j++){

            if(getCellule(i,j))
                cout<<"X";
            else
                cout<<"0";
        }
        cout<<endl;
    }
}*/

std::ostream& operator<<(std::ostream& f, const Etat2D& e){

    f<<"Grille 2D"<<endl;
    for (unsigned int i=0; i<e.getRow(); ++i){
        for (unsigned int j=0; j<e.getCol(); ++j)
            f<<e.getCellule(i,j);
        f<<endl;
    }
    return f;
}


Etat2D::Etat2D(const Etat2D& e) : nbLigne(e.nbLigne),nbColonne(e.nbColonne), valeurs(nullptr){

    if(nbLigne<=0 || nbColonne<=0)      //impossible dans tous les cas ! cf. le constructeur de Etat2D. Du coup si vous petes ok on supprime ce test
        throw AutomateException("Mauvaise dimension");
    else {
        valeurs = new bool*[nbLigne];
        for(unsigned int i=0; i<nbLigne; i++){

            valeurs[i]=new bool[nbColonne];
            for(unsigned int j; j<nbColonne; ++j)
                valeurs[i][j]=e.valeurs[i][j];
        }
    }
}

Etat2D& Etat2D::operator=(const Etat2D& e){

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

unsigned int Etat2D::CountVoisin(unsigned int li, unsigned int co)const{

    unsigned int nb=0;
    unsigned int limH, limB, limG, limD;

    if(li==0)
        limH=li;
    else limH=li-1;

    if(li==nbLigne)
        limB=li;
    else limB=li+1;

    if(co==0)
        limG=co;
    else limG=co-1;

    if(co==nbColonne)
        limD=co;
    else limD=co+1;

    for(int i=limH; i<=limB; i++){
        for(int j=limG; j<=limD; j++){
                if(valeurs[i][j])
                    nb+=1;
        }
    }
    //if(e.getCellule(li,co))
    if(valeurs[li][co])
        nb-=1;
    return nb;
}

void Automate2D::AppliquerTransition(const Etat2D& dep, Etat2D& dest) const{

    if(dest.getRow()!=dep.getRow()||dest.getCol()!=dep.getCol())
        dest=dep;

    for(unsigned int i=0; i<dep.getRow(); ++i){
        for(unsigned int j=0; j<dep.getCol(); ++j){

            unsigned int nbVoisin=dep.CountVoisin(i,j);

            if (dep.getCellule(i,j)){ //cellule vivante
                if(nbVoisin<nbMinVivant || nbVoisin>nbMaxVivant)
                    dest.setCellule(i,j,false); //si trop ou pas assez de voisin alors la cellule meurt
            }

            else {

                if(nbVoisin>=nbMinMort && nbVoisin<=nbMaxMort)
                    dest.setCellule(i,j,true); //si suffisament de cellule alors la cellule nait*/
            }
            }
        }
}


void Simulateur2D::setEtatDepart(const Etat2D& e){

    depart=&e;
    if(etats[0]==nullptr)
        etats[0]=new Etat2D(e);
    else *etats[0]=e;

}

Simulateur2D::Simulateur2D(const Automate2D& a, unsigned int buf):automate(a), depart(nullptr), nbMaxEtats(buf), rang(0){

        etats=new Etat2D*[nbMaxEtats];
        for(unsigned int i = 0; i<nbMaxEtats; i++)
            etats[i]=nullptr;
}

Simulateur2D::Simulateur2D(const Automate2D& a, const Etat2D& dep, unsigned int buffer):automate(a), depart(&dep), nbMaxEtats(buffer), rang(0){

        etats= new Etat2D*[nbMaxEtats];
        for(unsigned int i=0; i<nbMaxEtats; i++)
            etats[i]=nullptr;
        etats[0]=new Etat2D(*depart);
}

void Simulateur2D::next(){

    if(depart==nullptr)
        throw AutomateException("Etat de depart non defini");
    rang++;
    if(etats[rang%nbMaxEtats]==nullptr)
        etats[rang%nbMaxEtats]=new Etat2D;

    automate.AppliquerTransition(*etats[(rang-1)%nbMaxEtats],
                                 *etats[rang%nbMaxEtats]);

}

void Simulateur2D::run(unsigned int nbSteps){

    for(unsigned int i=0; i<nbSteps; i++)
        next();
}


void Simulateur2D::reset(){

    if (depart==nullptr)
        throw AutomateException("Etat de depart non defini");
    rang=0;
    *etats[0]=*depart;
}

const Etat2D& Simulateur2D::dernier() const{

if (depart==nullptr)
    throw AutomateException("Etat de depart non defini");
return *etats[rang%nbMaxEtats];
}

unsigned int Simulateur2D::getRangDernier()const{
    return rang;
}

Simulateur2D::~Simulateur2D(){

for(unsigned int i=0; i<nbMaxEtats; i++)
    delete etats[i];
delete[] etats;

}
