#ifndef MC_H
#define MC_H

#include "sample.h"
#include "stopping.h"


class MC:public Sample, public Stopping{
public:
    vector<vector<Atom> > record;

public:
    MC(string inFile);
    void run();
    double randMC();
};


#endif // MC_H
