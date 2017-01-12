#ifndef PLOTINFO_H
#define PLOTINFO_H

#include "global.h"
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include "atom.h"
#include <iostream>
#include <map>
#include <set>
#include "present.h"
using namespace std;


class PlotInfo{
public:
    double posL,posR;
    Vect plotDir;
    vector<Present> plotPresV;
    int slice;

    PlotInfo(){
        slice=100;
        plotPresV.clear();
        posL=0; posR=1;
        plotDir=Vect(1,0,0);
    }
};

#endif // PLOTINFO_H
