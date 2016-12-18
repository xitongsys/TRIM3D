#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "global.h"
#include "atom.h"

class Object3D {
public:
    //structure .obj file
    vector<Vect> points;
    vector<Vect> vnorms;
    vector<Face> faces;
    string name;
    double xmin,xmax,ymin,ymax,zmin,zmax;

    //elements
    vector<Atom> elements;
    vector<double> fraction;
    vector<double> density;// m-3

public:
    Object3D();
    void loadObj(string fname);
    void addElement(Atom type, double frac, double dens);

};

#endif // OBJECT3D_H
