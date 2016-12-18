#ifndef SAMPLE_H
#define SAMPLE_H

#include "global.h"
#include "object3d.h"

class Sample {

public:
    vector<Object3D> objs;

public:
    Sample();
    Sample(string input);
};

#endif // SAMPLE_H
