#include <iostream>
#include "autocell.h"
#include "automate1D.h"
#include "automate2D.h"

using namespace std;

int main()
{
    Automate2D a2(2,3,2,4);
    Etat e2(7,4);
    e2.setCellule(4,2,true);
    e2.setCellule(3,1,true);
    e2.setCellule(6,3,true);
    e2.setCellule(1,0,true);
    e2.setCellule(6,0,true);

    Simulateur s(a2,e2,10);
    s.run(8);

    int temp=0;
    /*for(Simulateur::ConstIterator it=s.getConstIterator();!it.isDone(); ++it){
            cout<<temp++<<"\n";
            cout<<*it;
            //cout<<s2.dernier();
            //s2.next();
            }*/
        for(unsigned int i=0;i<20;i++){

        s.next();
        cout<<s.dernier()<<"\n";

    }



    /*try {
    Automate1D a1(30);

    Etat e1(1,10);
    e1.setCellule(0,2,true); //faire attentio à initialiser à 0 la ligne
    e1.setCellule(0,6,true);
    e1.setCellule(0,7,true);
    e1.setCellule(0,9,true);

    Simulateur s2(a1,e1,10);
    s2.run(10);

    cout<<"Automate 1D"<<endl;
    //for(unsigned int i=0;i<10;i++){
    int temp=0;
    for(Simulateur::Iterator it=s2.getIterator();!it.isDone(); ++it){
            cout<<temp++<<"\n";
            cout<<*it;
            //cout<<s2.dernier();
            //s2.next();
            }
    }

    catch (AutomateException e){

        cout<<e.getInfo();
    }*/

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


    return 0;
}
