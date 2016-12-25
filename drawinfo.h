#ifndef DRAWINFO_H
#define DRAWINFO_H

#include <vector>
using namespace std;

class Color4f{

public:
    double r,g,b,a;

    Color4f(double rr, double gg, double bb, double aa){
        r = rr; g = gg; b = bb; a = aa;
    }
    Color4f(){
        r=0; g=0; b=0; a=1;
    }

};

class DrawInfo{
public:
    Color4f AtomColorTable[100];
    Color4f bgColor;
    Color4f objColor;

    double angleX, angleY, angleZ;
    double transX, transY, transZ;
    float MR[16];

    int atomSize, atomSlice;

    double randTmp(){
        int MOD=1000;
        double rnd = rand()%MOD;
        rnd /= MOD;
        return rnd;
    }

    DrawInfo(){
        for(int i=0; i<100; i++){
            AtomColorTable[i] = Color4f(randTmp()/2+0.5, randTmp()/2+0.5, randTmp()/2+0.5, 1.0);
        }

        bgColor=Color4f(0.0, 0.0, 0.0, 1.0);
        objColor=Color4f(0.5, 0.5, 0.5, 0.1);
        atomSize=2; atomSlice=5;
        angleX = 0; angleY = 0; angleZ=0;
        transX = 0; transY = 0; transZ = 0;
        memset(MR, 0, sizeof(MR));
        MR[0]=1; MR[5]=1; MR[10]=1; MR[15]=1;

    }

};

#endif // DRAWINFO_H
