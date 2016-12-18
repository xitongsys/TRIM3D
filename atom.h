#ifndef ATOM_H
#define ATOM_H

#include "global.h"
#include "phyconst.h"


enum IonType{ INCIDENT, RECOIL, TARGET};

class Atom{
public:
    string name;
    int Z;
    double mass, energy;
    double x,y,z,vx,vy,vz;

    IonType type;


public:
    Atom();
    Atom(string namep, int Zp, double massp,
         double xp, double yp, double zp,
         double vxp, double vyp, double vzp,
         IonType type);
};

#endif // ATOM_H
