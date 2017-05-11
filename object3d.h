#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "global.h"
#include "atom.h"

class Object3D {
public:
    //structure .obj file
    vector<Vect> points;//vertex of the structure
    vector<Vect> vnorms;//normal vector of every point
    vector<Face> faces;//faces
    string name;
    double xmin,xmax,ymin,ymax,zmin,zmax;//this obj range

    //elements
    vector<Atom> elements; //elements
    vector<double> fraction;// fraction of every elements
    vector<double> density;// atom density of every elements (atoms/A3)
    vector<double> disEnergy;// displacement energy of every kind of elements

    double ZAve,massAve;//average Z and average mass
    double densityAve;//total atom density
    double objDensity; //g/cm3
    double scale;//

public:
    Object3D();
    void loadObj(string fname);
    void addElement(Atom type, double frac, double dens, double disE);
    bool ifin(Vect &pos);
    Vect lineInteraction(Vect &pos, Vect &direct);

};

#endif // OBJECT3D_H
