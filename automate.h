#ifndef AUTOCELL_H_INCLUDED
#define AUTOCELL_H_INCLUDED

#include <string>
#include <iostream>
#include<typeinfo>
#include "etat.h"
#include <cstdlib> //pour le rand()

using namespace std;

class AutomateException {
public:
AutomateException(const std::string& message):info(message) {}
std::string getInfo() const { return info; }
private:
std::string info;
};

short unsigned int NumBitToNum(const std::string& num);
std::string NumToNumBit(short unsigned int num);


class Automate{
    unsigned int nbEtats;
    friend class AutomateManager;
public:
    virtual ~Automate()=default;
    Automate(unsigned int n) : nbEtats(n){}
    //void setNbEtat(unsigned int m){nbEtat=m;}
    unsigned int getNbEtats() const {return nbEtats;}
    virtual void AppliquerTransition(const Etat&, Etat&) const =0; //methode virtuelle pure --> la classe est abstraite donc non instanciable

};

class Automate1D : public Automate {
	unsigned short int numero;
	std::string numeroBit;
	Automate1D(unsigned short int num, unsigned int nbE=2);
	Automate1D(const std::string& num, unsigned int nbE=2);
    friend class AutomateManager;

public:
	unsigned short int getNumero() const { return numero; }
	const std::string& getNumeroBit() const { return numeroBit; }
	void AppliquerTransition(const Etat& dep, Etat& dest) const;

};

std::ostream& operator<<(std::ostream& f, const Automate1D& t);


class Automate2D : public Automate{
    unsigned int nbMinVivant; /*sachant une cellule vivante, cb il faut de cellule vivante minimum pour qu'elle vive à t+1*/
    unsigned int nbMaxVivant;
    unsigned int nbMinMort; /*même choix pour une cellule morte*/
    unsigned int nbMaxMort;
    Automate2D(unsigned int minV=0, unsigned int maxV=0, unsigned int minM=0, unsigned int maxM=0, unsigned int nbE=2);
    ~Automate2D()=default;
    friend class AutomateManager;

public :
    const unsigned int getMinV()const{return nbMinVivant;}
    const unsigned int getMaxV()const{return nbMaxVivant;}
    const unsigned int getMinM()const{return nbMinMort;}
    const unsigned int getMaxM()const{return nbMaxMort;}
	unsigned int CountVoisin(unsigned int li, unsigned int co, const Etat& e) const;
    void AppliquerTransition(const Etat& dep, Etat& dest) const;
};

std::ostream& operator<<(std::ostream& f, const Automate2D& A);



class AutomateEpidemie : public Automate {

	unsigned int chance1; //étant malade, chance /10 de mourir 
	unsigned int chance2; //étant sain, chance /10 de tomber malade
	
	friend class AutomateManager;

public : 
	AutomateEpidemie(unsigned int c1 = 0, unsigned int c2 = 0, unsigned int nbE = 4);
	~AutomateEpidemie() = default;
	const unsigned int getChance1() const { return chance1; }
	const unsigned int getChance2() const { return chance2; }
	unsigned int CountVoisinMalade(unsigned int li, unsigned int co, const Etat& e) const;
	void AppliquerTransition(const Etat& dep, Etat& dest) const;

};


std::ostream& operator<<(std::ostream& f, const AutomateEpidemie& A);


class AutomateManager{ //Le manager gère un ensemble d'automate qui peut être 1D ou 2D

    static AutomateManager* instance;
    Automate1D* automates1D[256];
    Automate2D** automates2D; //on ne créé pas un tableau de taille maximale car pas optimal
    unsigned int tailleTab2D=50;
    unsigned int nombre2DStockes=0; //le rang du dernier automate 2D rentré

    AutomateManager();
    ~AutomateManager();
    AutomateManager(const AutomateManager& A)=delete; //pour qu'on ne puisse pas l'utiliser car on ne veut pas d'autres instances
    AutomateManager& operator=(const AutomateManager& A)=delete; //idem

    struct Handler{
        AutomateManager* instance;
        Handler(): instance(nullptr){}
        ~Handler(){
            delete instance;}
        };

public:
    static Handler handler;
    static AutomateManager& getInstance();
    static void libererInstance(){
        delete handler.instance;
        handler.instance=nullptr;
    }
    const Automate1D& getAutomate1D(short unsigned int num);
    const Automate1D& getAutomate1D(const string& num);
    const Automate2D& getAutomate2D(unsigned int miniV, unsigned int maxiV, unsigned int miniM, unsigned int maxiM);
    unsigned int getDim2D()const {return tailleTab2D;}
    unsigned int getnombre2DStockes()const {return nombre2DStockes;}
    int indice_automate(unsigned int a, unsigned int b, unsigned int c, unsigned int d) const; //retourne l'indice dans le tableau ou -1 si jamais rentré

};

#endif // AUTOCELL_H_INCLUDED
