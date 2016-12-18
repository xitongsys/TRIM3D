#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "global.h"

class Object3D {
public:
    vector<Vect> points;
    vector<Vect> vnorms;
    vector<Face> faces;
    string name;

public:
    Object3D();
};

#endif // OBJECT3D_H
