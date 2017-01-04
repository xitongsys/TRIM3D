#ifndef DRAWINFO_H
#define DRAWINFO_H

#include "global.h"
#include <vector>
using namespace std;


class DrawInfo{
public:
    Color4f AtomColorTable[100];
    Color4f bgColor;
    Color4f objColor;

    double angleX, angleY, angleZ;
    double transX, transY, transZ;
    float MR[16];

    double atomSize, atomSlice;

    double randTmp(){
        int MOD=1000;
        double rnd = rand()%MOD;
        rnd /= MOD;
        return rnd;
    }

    DrawInfo(){
        for(int i=0; i<100; i++){
            AtomColorTable[i] = Color4f(randTmp(), randTmp(), randTmp(), 1.0);
        }

        bgColor=Color4f(0.8, 0.8, 0.8, 1.0);
        objColor=Color4f(0.1, 0.8, 0.8, 0.5);
        atomSize=2; atomSlice=5;
        angleX = 0; angleY = 0; angleZ=0;
        transX = 0; transY = 0; transZ = 0;
        memset(MR, 0, sizeof(MR));
        MR[0]=1; MR[5]=1; MR[10]=1; MR[15]=1;

    }

};

#endif // DRAWINFO_H
