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
    Vect pos;
    Vect direct;

    IonType type;


public:
    Atom();
    Atom(string namep, int Zp, double massp,
         Vect posp, Vect directp, double energyp,
         IonType typep);
};

#endif // ATOM_H
