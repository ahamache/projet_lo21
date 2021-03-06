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
/*! nbE est le nb d'état attribut de la classe mère, on utilise le constructeur de la classe mère*/

Automate1D::Automate1D(const std::string& num, unsigned int nbE) : Automate(nbE), numero(NumBitToNum(num)),numeroBit(num){}

void Automate1D::AppliquerTransition(const Etat& dep, Etat& dest) const {

    if (dep.getLongueur() > 1) /*! On vérifie que la grille de départ est à 1 dimension*/
        throw AutomateException("L'automate n'est pas 1D");

    if (dep.getLargeur() != dest.getLargeur())/*! On vérifie que les deux états sont de même dimension */
        dest = dep;

    /*! on détermine quelle transition effectuer en comptant le nombre de voisins vivants*/
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
    /*! on prend en compte les bords de la grille : par exemple une cellule située dans un angle a 3 voisins et pas 8*/
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

Automate2D::Automate2D(unsigned int minV, unsigned int maxV, unsigned int minM, unsigned int maxM) : Automate(2){

    if (minV>maxV || minM>maxV)/*! On vérifie que les valeurs entrées par l'utilisateur sont cohérentes*/
        throw AutomateException("minimum et maximum invalide");
    nbMinVivant=minV;
    nbMaxVivant=maxV;
    nbMinMort=minM;
    nbMaxMort=maxM;
}


void Automate2D::AppliquerTransition(const Etat& dep, Etat& dest) const{

    if (dep.getLongueur() < 2) /*! on vérifie que la grille de départ est à 2 dimensions */
        throw AutomateException("L'automate n'est pas 2D");

    if(dest.getLongueur()!=dep.getLongueur()||dest.getLargeur()!=dep.getLargeur()){ /*! On vérifie que les deux états sont de même dimension */
        dest=dep;
    }

    for(unsigned int i=0; i<dep.getLongueur(); i++){
        for(unsigned int j=0; j<dep.getLargeur(); j++){
            unsigned int nbVoisin=CountVoisin(i,j,dep); /*! Pour chaque cellule de la grille, on vérifie si au oins un voisin est malade puis on applique la règle de transition de l'automate*/

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


AutomateEpidemie::AutomateEpidemie(unsigned int c1, unsigned int c2) : Automate(4) {

    if (chance1>10 || chance2 >10) /*! On vérifie que les valeurs entrées par l'utilisateur sont bien des probabillités*/
        throw AutomateException("Probabilite superieure à 10");
    chance1=c1;
    chance2=c2;
}


unsigned int AutomateEpidemie::CountVoisinMalade(unsigned int li, unsigned int co, const Etat& e)const {
/*! on prend en compte les bords de la grille : par exemple une cellule située dans un angle a 3 voisins et pas 8*/
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
            if (e.getCellule(i, j)==1)
                return true;
        }
    }
//on ne fait pas de test sur la cellule elle-même car on appelle la fonction pour une cellule saine donc n'augmentera jamais le nb de malade
    return false;
}

void AutomateEpidemie::AppliquerTransition(const Etat& dep, Etat& dest) const {

    if (dep.getLongueur() < 2) /*! on vérifie que la grille de départ est à 2 dimensions */
        throw AutomateException("L'automate n'est pas 2D");

    if (dest.getLongueur() != dep.getLongueur() || dest.getLargeur() != dep.getLargeur()) { /*! On vérifie que les deux états sont de même dimension */
        dest = dep;
    }
/*! Pour chaque cellule de la grille, on compte le nombre de voisins sains et on applique la bonne transition*/
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



AutomateManager::AutomateManager() : tailleTab2D(50), nb2DStockes(0), tailleTabEp(50), nbEpStockes(0){
    //initialisation du tableau d'automate 1D
    for(unsigned int i=0; i<256; i++)
        automates1D[i]=nullptr;

    //initialisation du tableau d'automate 2D (jeu de la vie)
    automates2D = new Automate2D*[tailleTab2D];
    for(unsigned int i=0; i<tailleTab2D; i++)
        automates2D[i]=nullptr;

    //initialisation du tableau d'automate Epidémie
    automatesEp = new AutomateEpidemie*[tailleTabEp];
    for (unsigned int i = 0; i<tailleTabEp; i++)
        automatesEp[i] = nullptr;
}

AutomateManager::~AutomateManager(){
/*! On désalloue la mémoire allouée dynamiquement à l'AutomateManager*/
    for(unsigned int i=0; i<256; i++)
        delete automates1D[i];

    for(unsigned int i=0; i<tailleTab2D; i++)
        delete automates2D[i];

    for (unsigned int i = 0; i<tailleTabEp; i++)
        delete automatesEp[i];
}

AutomateManager& AutomateManager::getInstance(){

    if(handler.instance==nullptr)
        handler.instance=new AutomateManager;
    return *handler.instance;
}

AutomateManager::Handler AutomateManager::handler = AutomateManager::Handler();

const Automate1D& AutomateManager::getAutomate1D(short unsigned int num){

    if(num>255)/*! On vérifie que l'indice entré par l'utilisateur n'est pas supérieur à 255*/
        throw AutomateException("Automate inexistant");
    if (automates1D[num]==nullptr)
        automates1D[num]=new Automate1D(num);
    return *automates1D[num];
}

const Automate1D& AutomateManager::getAutomate1D(const string& num){

    return getAutomate1D(NumBitToNum(num));
}

int AutomateManager::indice_automate2D(unsigned int a, unsigned int b, unsigned int c, unsigned int d) const{

    for(unsigned int i=0;i<getNb2DStockes();i++){ /*! on inspecte tout les éléments jusqu'au dernier; la recherche est séquentielle donc la complexité n'est pas très bonne*/

        if(automates2D[i]->getMinV()==a && automates2D[i]->getMaxV()==b && automates2D[i]->getMinM()==c && automates2D[i]->getMaxM()==d)
            return i;
    }

    return -1; //l'automate n'existe pas dans le tableau
}

const Automate2D& AutomateManager::getAutomate2D(unsigned int miniV, unsigned int maxiV, unsigned int miniM, unsigned int maxiM){

    int indice=indice_automate2D(miniV, maxiV, miniM, maxiM);

    if(indice==-1){ /*! Si l'automate n'a jamais été rentré dans le tableau automates2D[], on l'ajoute à la fin du tableau*/

        if(nb2DStockes==tailleTab2D){ /*! Si le tableau est complet, on l'agrandit de 10 cases supplémentaires*/

            Automate2D** newtab=new Automate2D*[tailleTab2D + 10];

            for(unsigned int i=0;i<tailleTab2D;i++)
                newtab[i]=automates2D[i];

            Automate2D** old=automates2D;
            automates2D = newtab;
            tailleTab2D+=10;
            delete[] old;
        }

        automates2D[nb2DStockes]=new Automate2D(miniV, maxiV, miniM, maxiM);
        return *automates2D[nb2DStockes++];
    }
    return *automates2D[indice];
}


int AutomateManager::indice_automateEp(unsigned int a, unsigned int b) const {

    for (unsigned int i = 0; i<getNbEpStockes(); i++) { /*! on inspecte tous les éléments stockés dans le tableau jusqu'au dernier; la recherche est séquentielle donc la complexité n'est pas très bonne*/

        if (automatesEp[i]->getChance1() == a && automatesEp[i]->getChance2() == b)
            return i;
    }

    return -1; //l'automate n'existe pas dans le tableau
}


const AutomateEpidemie& AutomateManager::getAutomateEp(unsigned int c1, unsigned int c2) {

    int indice = indice_automateEp(c1, c2);

    if (indice == -1) { /*! Si l'automate n'a jamais été rentré dans le tableau automatesEp[], on l'ajoute à la fin du tableau*/

        if (nbEpStockes == tailleTabEp) { /*! Si le tableau est complet, on l'agrandit de 10 cases supplémentaires*/

            AutomateEpidemie** newtab = new AutomateEpidemie*[tailleTabEp + 10];

            for (unsigned int i = 0; i<tailleTabEp; i++)
                newtab[i] = automatesEp[i];

            AutomateEpidemie** old = automatesEp;
            automatesEp = newtab;
            tailleTabEp += 10;
            delete[] old;
        }

        automatesEp[nbEpStockes] = new AutomateEpidemie(c1, c2);
        return *automatesEp[nbEpStockes++];
    }
    return *automatesEp[indice];

}

const Automate1D* AutomateManager::getElem1D(unsigned int n) const { //permet de retourner l'adresse de l'element du tableau automates1D à l'indice n
/*! On vérifie que l'indice entré par l'utilisateur n'est pas supérieur à 255*/
    if(n>255)
        throw AutomateException("L'automate n'existe pas");
    return automates1D[n];
}

const Automate2D* AutomateManager::getElem2D(unsigned int n) const { //permet de retourner l'élément du tableau automates2D à l'indice n
/*! On vérifie que l'indice entré par l'utilisateur n'est pas supérieur à nb2DStockes*/
    if(n>nb2DStockes)
        throw AutomateException("L'automate n'existe pas");
    return automates2D[n];
}

const AutomateEpidemie* AutomateManager::getElemEp(unsigned int n) const {
/*! On vérifie que l'indice entré par l'utilisateur n'est pas supérieur à nbEpStockes*/
    if(n>nbEpStockes)
        throw AutomateException("L'automate n'existe pas");
    return automatesEp[n];
}
