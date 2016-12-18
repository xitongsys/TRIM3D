#include "atom.h"

Atom::Atom(){
    name="H";
    Z=1; mass=1.008; energy=0;
    x=y=z=0;
    vx=vy=vz=0;
    type = TARGET;
}

Atom::Atom(string namep, int Zp, double massp,
           double xp, double yp, double zp,
           double vxp, double vyp, double vzp,
           IonType typep){

    name = namep;
    Z = Zp;
    mass = massp;
    x = xp; y = yp; z = zp;
    vx = vxp; vy = vyp; vz = vzp;

    type = typep;

}






