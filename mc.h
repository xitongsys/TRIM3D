#ifndef MC_H
#define MC_H

#include "sample.h"
#include "stopping.h"


class MC:public Sample, public Stopping{
public:
    vector<vector<Atom> > record;
    int incidentNum,recoilNum;
    int runMode;

public:
    MC(string inFile, string sFile);
   // void run();
    int runOne(int i, int j);
    double randMC();
};


#endif // MC_H
