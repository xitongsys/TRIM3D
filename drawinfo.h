#ifndef DRAWINFO_H
#define DRAWINFO_H

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
#include <string.h>
#include <cstdio>
using namespace std;


class DrawInfo{
public:
    vector<Present> pres;

    Color4f bgColor;

    double angleX, angleY, angleZ;
    double transX, transY, transZ;
    float MR[16];

    double orthDX;//for orth projection

    int ifDrawAxesLine, ifDrawAxes3D;
    int ifShow;
    int ifDrawSelectBox;
    int projType;

    DrawInfo(){

        bgColor=Color4f(0.8, 0.8, 0.8, 1.0);
        pres.push_back(Present("T==0", Color4f(1,0,0,1),3,4));
        pres.push_back(Present("T==1", Color4f(0,1,1,1),1,4));
        pres.push_back(Present("T==3", Color4f(0.5,0.5,0.5,0.5),2,4));
        pres.push_back(Present("T==4", Color4f(0,0,1,1),2,4));

        pres.push_back(Present("obj>=0",Color4f(0.5, 1, 0.5, 0.5),2.0, 4));

        angleX = 0; angleY = 0; angleZ=0;
        transX = 0; transY = 0; transZ = 0;
        memset(MR, 0, sizeof(MR));
        MR[0]=1; MR[5]=1; MR[10]=1; MR[15]=1;

        orthDX=0;

        ifDrawAxes3D=1; ifDrawAxesLine=1;
        ifShow=1;
        ifDrawSelectBox=1;
        projType=1;

    }

};

#endif // DRAWINFO_H







