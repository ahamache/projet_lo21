#include "automate.h"

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

Automate1D::Automate1D(unsigned short int num, unsigned int nbE): Automate(nbE), numero(num),numeroBit(NumToNumBit(num)){}
//nbE est le nb d'état attribut de la classe mère, on utilise le constructeur de la classe mère

Automate1D::Automate1D(const std::string& num, unsigned int nbE) : Automate(nbE), numero(NumBitToNum(num)),numeroBit(num){}

void Automate1D::AppliquerTransition(const Etat& dep, Etat& dest) const {

    if (dep.getLongueur() > 1) //on vérifie que l'automate est 1D
        throw AutomateException("L'automate n'est pas 1D");

	if (dep.getLargeur() != dest.getLargeur())
        dest = dep;

	for (unsigned int i = 0; i < dep.getLargeur(); i++) {
		unsigned short int conf=0;

		if (i > 0) //quand il y a un voisin à gauche
            conf+=dep.getCellule(0, i - 1) * 4;

		conf+=dep.getCellule(0,i)*2; //cellule elle même

		if (i < dep.getLongueur()-1) //quand il y a un voisin à droite
            conf+=dep.getCellule(0, i + 1);
		dest.setCellule(0, i, numeroBit[7-conf]-'0');
	}
}

std::ostream& operator<<(std::ostream& f, const Automate1D& A) {
	f << A.getNumero() << " : " << A.getNumeroBit() << "\n";
	return f;
}


unsigned int Automate2D::CountVoisin(unsigned int li, unsigned int co, const Etat& e)const{

    unsigned int nb=0;
    unsigned int limH, limB, limG, limD;

    if(li==0)
        limH=li;
    else limH=li-1;

    if(li==e.getLongueur()-1)
        limB=li;
    else limB=li+1;

    if(co==0)
        limG=co;
    else limG=co-1;

    if(co==e.getLargeur()-1)
        limD=co;
    else limD=co+1;

    for(unsigned int i=limH; i<=limB; i++){
        for(unsigned int j=limG; j<=limD; j++){
                if(e.getCellule(i,j)==1)
                    nb+=1;
        }
    }
    if(e.getCellule(li, co))
        nb-=1;
    return nb;
}

Automate2D::Automate2D(unsigned int minV, unsigned int maxV, unsigned int minM, unsigned int maxM, unsigned int nbE) : Automate(nbE){ 
	//nbE est le nb d'état attribut de la classe mère

    if (minV>maxV || minM>maxV)
        throw AutomateException("minimum et maximum invalide");
    nbMinVivant=minV;
    nbMaxVivant=maxV;
    nbMinMort=minM;
    nbMaxMort=maxM;
}


void Automate2D::AppliquerTransition(const Etat& dep, Etat& dest) const{

    if (dep.getLongueur() < 2) //on vérifie que l'automate est 2D
        throw AutomateException("L'automate n'est pas 2D");

    if(dest.getLongueur()!=dep.getLongueur()||dest.getLargeur()!=dep.getLargeur()){
        dest=dep;
    }

    for(unsigned int i=0; i<dep.getLongueur(); i++){
        for(unsigned int j=0; j<dep.getLargeur(); j++){
            unsigned int nbVoisin=CountVoisin(i,j,dep);

            if (dep.getCellule(i,j)==1){ //cellule vivante
                if(nbVoisin<nbMinVivant || nbVoisin>nbMaxVivant)
                    dest.setCellule(i,j,0); //si trop ou pas assez de voisin alors la cellule meurt
            }

            else {

                if(nbVoisin>=nbMinMort && nbVoisin<=nbMaxMort)
                    dest.setCellule(i,j,1); //si suffisament de cellule alors la cellule nait*/
            }
            }
        }
}

std::ostream& operator<<(std::ostream& f, const Automate2D& A) {
	f <<"Regle cellule vivante :"<<endl<<"Nombre minimum = " << A.getMinV() <<endl<<"Nombre maximum"<<A.getMaxV()
	<<"Regle cellule morte :"<<endl<<"Nombre minimum = " << A.getMinM() <<endl<<"Nombre maximum"<<A.getMaxM();

	return f;
}

std::ostream& operator<<(std::ostream& f, const AutomateEpidemie& A) {

	f << "Etant malade, une cellule a " << A.getChance1() << "/10 de mourir" << endl;
	f << "Etant saine, une cellule a " << A.getChance2() << "/10 de tomber malade" << endl;
	return f;
}


AutomateEpidemie::AutomateEpidemie(unsigned int c1, unsigned int c2, unsigned int nbE) : Automate(nbE), chance1(c1), chance2(c2) {}

/*
0 -> saine //la grille est initialisée à saine et ensuite on met quelques malades sur la grille 
1 -> malade
2 -> immunisé
3 -> mort
*/

unsigned int AutomateEpidemie::CountVoisinMalade(unsigned int li, unsigned int co, const Etat& e)const {

	unsigned int nbM = 0;
	unsigned int limH, limB, limG, limD;

	if (li == 0)
		limH = li;
	else limH = li - 1;

	if (li == e.getLongueur() - 1)
		limB = li;
	else limB = li + 1;

	if (co == 0)
		limG = co;
	else limG = co - 1;

	if (co == e.getLargeur() - 1)
		limD = co;
	else limD = co + 1;

	for (unsigned int i = limH; i <= limB; i++) {
		for (unsigned int j = limG; j <= limD; j++) {
			if (e.getCellule(i, j)==1) //la cellule possède au moins un voisin malade
				return true;
		}
	}
//on ne fait pas de test sur la cellule elle-même car on appelle la fonction pour une cellule saine donc n'augmentera jamais le nb de malade
	return false;
}

void AutomateEpidemie::AppliquerTransition(const Etat& dep, Etat& dest) const {

	if (dep.getLongueur() < 2) //on vérifie que l'automate est 2D
		throw AutomateException("L'automate n'est pas 2D");

	if (dest.getLongueur() != dep.getLongueur() || dest.getLargeur() != dep.getLargeur()) {
		dest = dep;
	}

	for (unsigned int i = 0; i < dep.getLongueur(); i++) {
		for (unsigned int j = 0; j < dep.getLargeur(); j++) {

			if (dep.getCellule(i, j) == 1) { //la cellule est malade

				unsigned int p = rand() % 11; //on veut un chiffre entre 0 et 10
				if (p <= chance1) //si le chiffre obtenu est inférieur ou égal à la chance de mourir alors elle meurt
					dest.setCellule(i, j, 3);
				else dest.setCellule(i, j, 2); //la cellule devient immunisée
			}

			if (dep.getCellule(i, j) == 0) { //la cellule est saine 
				unsigned int nbVoisinM = CountVoisinMalade(i, j, dep);
				if (nbVoisinM > 0) { //la cellule a au moins un voisin malade
					unsigned int p = rand() % 11;
					if (p <= chance2)
						dest.setCellule(i, j, 1);
					//sinon reste dans le même état sain
				}
			}
			//sinon une cellule morte reste morte et une immunisée reste immunisée
		}
	}
}



AutomateManager::AutomateManager() : tailleTab2D(50), nombre2DStockes(0){

    for(unsigned int i=0; i<256; i++)
        automates1D[i]=nullptr;
    automates2D = new Automate2D*[tailleTab2D];
    for(unsigned int i=0; i<tailleTab2D; i++)
        automates2D[i]=nullptr;
}

AutomateManager::~AutomateManager(){

    for(unsigned int i=0; i<256; i++)
        delete automates1D[i];

    for(unsigned int i=0; i<tailleTab2D; i++)
        delete automates2D[i];
}

AutomateManager& AutomateManager::getInstance(){

    if(handler.instance==nullptr)
        handler.instance=new AutomateManager;
    return *handler.instance;
}

AutomateManager::Handler AutomateManager::handler = AutomateManager::Handler();

const Automate1D& AutomateManager::getAutomate1D(short unsigned int num){

    if(num>255)
        throw AutomateException("Automate inexistant");
    if (automates1D[num]==nullptr)
        automates1D[num]=new Automate1D(num);
    return *automates1D[num];
}

const Automate1D& AutomateManager::getAutomate1D(const string& num){

    return getAutomate1D(NumBitToNum(num));
}

int AutomateManager::indice_automate(unsigned int a, unsigned int b, unsigned int c, unsigned int d) const{

    for(unsigned int i=0;i<getnombre2DStockes();i++){ //on inspecte tout les éléments jusqu'au dernier

        if(automates2D[i]->getMinV()==a && automates2D[i]->getMaxV()==b && automates2D[i]->getMinM()==c && automates2D[i]->getMaxM()==d)
            return i;
    }

    return -1; //l'automate n'existe pas dans le tableau
}

const Automate2D& AutomateManager::getAutomate2D(unsigned int miniV, unsigned int maxiV, unsigned int miniM, unsigned int maxiM){

    int indice=indice_automate(miniV, maxiV, miniM, maxiM);

    if(indice==-1){ //l'automate n'a jamais été rentré dans le tableau automates2D[]

        if(nombre2DStockes==tailleTab2D){ //le tableau est complet, il faut l'agrandir

            Automate2D** newtab=new Automate2D*[tailleTab2D + 10];

            for(unsigned int i=0;i<tailleTab2D;i++)
                newtab[i]=automates2D[i];

            Automate2D** old=automates2D;
            automates2D = newtab;
            tailleTab2D+=10;
            delete[] old;
        }

        automates2D[nombre2DStockes]=new Automate2D(miniV, maxiV, miniM, maxiM);
        return *automates2D[nombre2DStockes++];
    }
    return *automates2D[indice];
}

