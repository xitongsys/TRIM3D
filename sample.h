#ifndef SAMPLE_H
#define SAMPLE_H

#include "global.h"
#include "object3d.h"

class Sample{

public:
    vector<Object3D> objs;
    vector<Atom> ions;
    vector<int> ionNum;
    double xmin,xmax,ymin,ymax,zmin,zmax;

public:
    Sample();
    void loadInput(string input);
};

#endif // SAMPLE_H
