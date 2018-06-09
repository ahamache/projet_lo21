#ifndef AUTOCELL_H_INCLUDED
#define AUTOCELL_H_INCLUDED

#include <string>
#include <iostream>
#include<typeinfo>


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

class Etat{

    unsigned int nbColonne;
    unsigned int nbLigne; // =1 quand 1D
    bool** valeurs;

public:
    Etat(unsigned int c, unsigned int l);
    Etat() : nbLigne(0), nbColonne(0), valeurs(nullptr) {}
    unsigned int getLargeur()const {return nbColonne;}
    unsigned int getLongueur()const {return nbLigne;}
    //utile de faire une fonction quelle_dim ?
    ~Etat();
    bool getCellule(unsigned int i, unsigned int j) const;
    void setCellule(unsigned int i, unsigned int j, bool val);
	Etat(const Etat& e);
	Etat& operator=(const Etat& e);

};

std::ostream& operator<<(std::ostream& f, const Etat& e);


class Automate{

    unsigned int nbEtat; //pour l'instant seulement 2

public:
    Automate(unsigned int n=1) :  nbEtat(n){}
    void setNbEtat(unsigned int m){nbEtat=m;}
    unsigned short int getNbEtat() const {return nbEtat;}
    virtual void AppliquerTransition(const Etat&, Etat&) const =0; //methode virtuelle pure
    ~Automate()=default;
};


class Simulateur{

    const Automate& automate;
    const Etat* depart;
    Etat** etats;
    unsigned int nbMaxEtats; //nb max qu'on sauvegarde dans le tableau etats
    unsigned int rang;
    void build(unsigned int c);
    Simulateur(const Simulateur& s)=delete;
    Simulateur& operator=(const Simulateur& s)=delete;
public :
    Simulateur(const Automate& a, unsigned int buf=2);
    Simulateur(const Automate& a, const Etat& dep, unsigned int buffer=2);
    void setEtatDepart(const Etat& e);
    void next();
    void run(unsigned int nbSteps);
    void reset();
    const Etat& dernier() const;
    unsigned int getRangDernier()const{return rang;}
    ~Simulateur();
    /*class iterator {
		friend class Simulateur;
        Simulateur* sim;
        int i;
		iterator(Simulateur* s) :sim(s), i(s->rang) {}
		iterator(Simulateur* s, int dep) :sim(s), i(dep) {}
	public:
        iterator():sim(nullptr),i(0) {}
		bool isDone() const {
			return ((sim == nullptr) ||
				   (i == -1 && sim->rang<sim->nbMaxEtats) ||
				   (i == sim->rang - sim->nbMaxEtats));
		}
		iterator& operator++() {
			if (isDone())
				throw AutomateException("error, next on an iterator which is done");
			i--;
			if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
			return *this;
		}
		Etat& operator*() const {
			if (isDone())
				throw AutomateException("error, indirection on an iterator which is done");
			return *sim->etats[i%sim->nbMaxEtats];
		}
		bool operator!=(iterator it) const { return sim != it.sim || i != it.i; }
	};
	iterator begin() {	return iterator(this,this->rang); }
	iterator end() { if (rang < nbMaxEtats) return iterator(this, -1); else return iterator(this, rang - nbMaxEtats);  }
	class const_iterator {
		friend class Simulateur;
        const Simulateur* sim;
        int i;
		const_iterator(const Simulateur* s) :sim(s), i(s->rang) {}
		const_iterator(const Simulateur* s, int dep) :sim(s), i(dep) {}
	public:
        const_iterator():sim(nullptr),i(0) {}
		bool isDone() const {
			return sim==nullptr || (i == -1 && sim->rang<sim->nbMaxEtats) ||
				i== sim->rang - sim->nbMaxEtats;
		}
		const_iterator& operator++() {
			if (isDone())
				throw AutomateException("error, next on an iterator which is done");
			i--;
			if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
			return *this;
		}
		const Etat& operator*() const {
			if (isDone())
				throw AutomateException("error, next on an iterator which is done");
			return *sim->etats[i%sim->nbMaxEtats];
		}
		bool operator!=(const_iterator it) const { return sim != it.sim || i != it.i; }
	};
	const_iterator cbegin() const { return const_iterator(this); }
	const_iterator cend() const { if (rang < nbMaxEtats) return const_iterator(this, -1); else return const_iterator(this, rang - nbMaxEtats); }
    */
    class Iterator {
		friend class Simulateur;
        Simulateur* sim;
        int i;
		Iterator(Simulateur* s):sim(s), i(s->rang){}
	public:
        Iterator():sim(nullptr),i(0) {}
		bool isDone() const {
			return ((sim == nullptr) ||
				   (i == -1 && sim->rang<sim->nbMaxEtats) ||
				   (i == sim->rang - sim->nbMaxEtats));
		}
		Iterator& operator++() {
			if (isDone())
				throw AutomateException("error, next on an iterator which is done");
			i--;
			if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
		}
		Etat& operator* () const {
			if (isDone())
				throw AutomateException("error, indirection on an iterator which is done");
			return *sim->etats[i%sim->nbMaxEtats];
		}
	};
	Iterator getIterator() {
		return Iterator(this);
	}
};

#endif // AUTOCELL_H_INCLUDED
