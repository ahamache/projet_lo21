#ifndef SIMULATEUR_H_INCLUDED
#define SIMULATEUR_H_INCLUDED

#include "etat.h"
#include "automate.h"

using namespace std;

class Simulateur{

    const Automate& automate;
    const Etat* depart;
    Etat** etats;
    unsigned int nbMaxEtats=0; //nb max qu'on sauvegarde dans le tableau etats
    unsigned int rang=0;
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

    class Iterator {
		friend class Simulateur;
        Simulateur* sim;
        const unsigned int i;
		int j;
		Iterator(Simulateur* s);
	public:
		Iterator();
		bool isDone() const;
		Iterator& operator++();
		Etat& operator* () const;
	};
	Iterator getIterator() {
		return Iterator(this);
	}

	class ConstIterator {
		friend class Simulateur;
        const Simulateur* sim;
        const unsigned int i;
		int j;
		ConstIterator(Simulateur* s);
	public:
		ConstIterator();
		bool isDone() const;
		ConstIterator& operator++();
		Etat& operator* () const;
	};
	ConstIterator getConstIterator() {
		return ConstIterator(this);
	}
};


#endif // SIMULATEUR_H_INCLUDED
