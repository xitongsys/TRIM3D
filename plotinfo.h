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
    Vect posL,dirL, posR, dirR;
    vector<Present> plotPresV;
    int slice;

    PlotInfo(){
        slice=100;
        plotPresV.clear();
        posL=Vect(0,0,0); posR=Vect(0,0,0);
        dirL=Vect(1,0,0); dirR=Vect(-1,0,0);
    }
};

#endif // PLOTINFO_H
