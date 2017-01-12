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
        plotPresV.push_back(Present("T==0", Color4f(1,0,0,0.5),0,0));
        plotPresV.push_back(Present("T==1", Color4f(0,1,1,0.5),0,0));
        plotPresV.push_back(Present("T==3", Color4f(0.5,0.5,0.5,0.5),0,0));
        plotPresV.push_back(Present("T==4", Color4f(0,0,1,0.5),0,0));

        posL=0; posR=1;
        plotDir=Vect(1,0,0);
    }
};

#endif // PLOTINFO_H
