#ifndef AUTOMATE1D_H_INCLUDED
#define AUTOMATE1D_H_INCLUDED

#include "autocell.h"


class Automate1D : public Automate {
	unsigned short int numero;
	std::string numeroBit;
	//friend class AutomateManager;
public:
    Automate1D(unsigned short int num, unsigned int nb=0);
	Automate1D(const std::string& num, unsigned int nb=0);

    //Automate1D(const Automate1D& a);
    //Automate1D& operator=(const Automate1D& a);
	unsigned short int getNumero() const { return numero; }
	const std::string& getNumeroBit() const { return numeroBit; }
	void AppliquerTransition(const Etat& dep, Etat& dest) const;
};

std::ostream& operator<<(std::ostream& f, const Automate1D& t);


#endif // AUTOMATE1D_H_INCLUDED
