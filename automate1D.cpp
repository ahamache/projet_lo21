#include "automate1D.h"


Automate1D::Automate1D(unsigned short int num, unsigned int nb):Automate(nb), numero(num),numeroBit(NumToNumBit(num)){
}

Automate1D::Automate1D(const std::string& num, unsigned int nb) : Automate(nb), numero(NumBitToNum(num)),numeroBit(num) {
}

void Automate1D::appliquerTransition(const Etat& dep, Etat& dest) const {

    if (dep.getHauteur() > 1) //on vérifie que l'automate est 1D
        throw AutomateException("L'automate n'est pas 1D");

	if (dep.getHauteur() != dest.getHauteur()) dest = dep;

	for (unsigned int i = 0; i < dep.getHauteur(); i++) {
		unsigned short int conf=0;

		if (i > 0) conf+=dep.getCellule(1, i - 1) * 4;
		conf+=dep.getCellule(1,i)*2;

		if (i < dep.getHauteur()-1) conf+=dep.getCellule(1, i + 1);
		dest.setCellule(1, i, numeroBit[7-conf]-'0');
	}
}

std::ostream& operator<<(std::ostream& f, const Automate1D& A) {
	f << A.getNumero() << " : " << A.getNumeroBit() << "\n";
	return f;
}
