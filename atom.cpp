#include "atom.h"

Atom::Atom(){
    name="H";
    Z=1; mass=1.008; energy=0;
    pos=Vect(0,0,0);
    direct=Vect(0,0,1);
    type = TARGET;
}

Atom::Atom(string namep, int Zp, double massp,
           Vect posp, Vect directp, double energyp,
           IonType typep){

    name = namep;
    Z = Zp;
    mass = massp;
    pos = posp;
    direct = directp;
    energy = energyp;

    type = typep;

}






