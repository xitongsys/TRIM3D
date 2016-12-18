#ifndef SAMPLE_H
#define SAMPLE_H

#include "global.h"
#include "object3d.h"

class Sample {

public:
    vector<Object3D> objs;
    double xmin,xmax,ymin,ymax,zmin,zmax;

public:
    Sample();
    void loadFile(char *fname);
};

#endif // SAMPLE_H
