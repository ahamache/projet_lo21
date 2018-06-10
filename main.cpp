#include <iostream>
//#include "autocell.h"
//#include "automate1D.h"
#include "simulateur.h"

using namespace std;

int main()
{
	const AutomateEpidemie a3 = AutomateManager::getInstance().getAutomateEp(7, 5);
	Etat e3(10, 10);
	e3.setCellule(2, 3, 1);
	e3.setCellule(4, 4, 1);
	e3.setCellule(0, 0, 1);
	e3.setCellule(3, 1, 1);
	e3.setCellule(7, 6, 1);
	e3.setCellule(9, 8, 1);
	e3.setCellule(5, 5, 1);
	Etat e4 = e3;
	std::cout << e3 << "\n";
	Simulateur s(a3, e3, 10);
	/*for (unsigned int i = 1;i <= 15;i++) {
		cout << "next numero " << i << "\n";
		s.next();
		cout << s.dernier();
	}*/

	s.run(10);
	//std::cout << "rang = "<<s.getRangDernier()<<"\n"<<"generation" << std::endl;
	int j = 0;
	for (Simulateur::ConstIterator it = s.getConstIterator(); !it.isDone(); ++it)
		std::cout << "iteration n° "<<++j << "\n" << *it << "\n\n";
	//std::cout << e4 << "\n";


	/*Simulateur s3(a3, e3, 10);
	for (unsigned int i = 0; i<10; i++) {
		cout << s3.dernier() << "\n";
		s3.next();
	}*/


    /*const Automate2D& a2=AutomateManager::getInstance().getAutomate2D(2,3,2,4);
    Etat e2(10,10);
    e2.setCellule(4,2,true);
    e2.setCellule(3,1,true);
    e2.setCellule(6,3,true);
    e2.setCellule(1,0,true);
    e2.setCellule(6,0,true);

    Simulateur s(a2,e2,10);
    s.run(15);
    for(Simulateur::ConstIterator it=s.getConstIterator(); !it.isDone(); ++it)
        std::cout<<*it<<"\n";*/


    /*for(unsigned int i=0;i<10;i++){
            cout<<s.dernier()<<"\n";
            s.next();}

    try {
    Automate1D a1(30);

    Etat e1(1,10);
    e1.setCellule(0,2,true); //faire attentio à initialiser à 0 la ligne
    e1.setCellule(0,6,true);
    e1.setCellule(0,7,true);
    e1.setCellule(0,9,true);

    Simulateur s2(a1,e1);

    cout<<"Automate 1D"<<endl;

    for(unsigned int i=0;i<10;i++){
            cout<<s2.dernier();
            s2.next();}
    }

    catch (AutomateException e){

        cout<<e.getInfo();
    }

    /*cout << "Creation d'etats" << endl;
    Etat1D e1(15);
    e1.setCellule(4,true);
    e1.setCellule(3,true);
    e1.setCellule(6,true);
    e1.setCellule(1,true);
    e1.setCellule(13,true);
    e1.setCellule(11,true);
    e1.setCellule(10,true);
    e1.setCellule(0,true);
    e1.setCellule(8,true);

    Automate1D a=Automate1D(25);

    Simulateur1D s(a,e1);
    cout<<e1;
    for(unsigned int i=0;i<10;i++){

        s.next();
        cout<<s.dernier()<<"\n";

    }*/

	system("pause");

    return 0;
}
