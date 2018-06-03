#include <iostream>
#include "autocell.h"

using namespace std;

int main()
{
    Automate2D a(2,3,2,4);

    cout << "Creation d'etats" << endl;
    Etat2D e1(7,4);
    e1.setCellule(4,2,true);
    e1.setCellule(3,1,true);
    e1.setCellule(6,3,true);
    e1.setCellule(1,0,true);
    e1.setCellule(6,0,true);

    Simulateur2D s(a,e1);
    cout<<e1;
    for(unsigned int i=0;i<10;i++){

        s.next();
        cout<<s.dernier()<<"\n";

    }

    return 0;
}
