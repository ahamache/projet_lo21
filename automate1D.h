#ifndef AUTOMATE1D_H_INCLUDED
#define AUTOMATE1D_H_INCLUDED

#include "autocell.h"

short unsigned int NumBitToNum(const std::string& num);
std::string NumToNumBit(short unsigned int num);

class Etat1D : public Etat {

public:
	Etat1D():nbColonnes(0), valeur(nullptr) {}
	Etat1D(unsigned int n);
	~Etat1D() { delete[] valeur; }
	Etat1D(const Etat1D& e);
	Etat1D& operator=(const Etat1D& e);
	void setCellule(unsigned int i, bool val);
	bool getCellule(unsigned int) const;
	unsigned int getNbColonnes() const { return nbColonnes; }
};

std::ostream& operator<<(std::ostream& f, const Etat1D& e);


class Automate1D :public Automate {
	unsigned short int numero;
	std::string numeroBit;
	//friend class AutomateManager;
public:
    Automate1D(unsigned short int num);
	Automate1D(const std::string& num);
    ~Automate1D(){}
    Automate1D(const Automate1D& a);
    Automate1D& operator=(const Automate1D& a);
	unsigned short int getNumero() const { return numero; }
	const std::string& getNumeroBit() const { return numeroBit; }
	void appliquerTransition(const Etat1D& dep, Etat1D& dest) const;
};

std::ostream& operator<<(std::ostream& f, const Automate1D& t);


class Simulateur1D : public Simulateur{
	const Automate1D& automate;
    Etat1D** etats;
    const Etat1D* depart;
	unsigned int nbMaxEtats;
    unsigned int rang;
	void build(unsigned int c);
    Simulateur1D(const Simulateur1D& s);
    Simulateur1D& operator=(const Simulateur1D& s);
public:
	Simulateur1D(const Automate1D& a, unsigned int buffer = 2);
	Simulateur1D(const Automate1D& a, const Etat1D& dep, unsigned int buffer = 2);
	void setEtatDepart(const Etat1D& e);
	void run(unsigned int nbSteps); // génère les n prochains états
	void next(); // génère le prochain état
	const Etat1D& dernier() const;
	unsigned int getRangDernier() const { return rang; }
	void reset(); // revenir à l'état de départ
	~Simulateur1D();
};

#endif // AUTOMATE1D_H_INCLUDED
