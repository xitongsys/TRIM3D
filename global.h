#ifndef GLOBAL_H
#define GLOBAL_H

#include <bits/stdc++.h>
#include <fstream>

using namespace std;

struct Vect{
    double x,y,z;
    Vect(double xx, double yy, double zz){
        x = xx; y = yy; z = zz;
    }
    Vect(){x=1; y=0; z=0;}

    Vect operator^(Vect & v2){
        double x2 = v2.x, y2 = v2.y, z2 = v2.z;
        Vect va;
        va.x = y*z2 - z*y2;
        va.y = z*x2 - z2*x;
        va.z = x*y2 - x2*y;
        return va;
    }

    Vect operator-(Vect &v2){
        Vect va;
        va.x = x - v2.x;
        va.y = y - v2.y;
        va.z = z - v2.z;
        return va;
    }

    Vect operator+(Vect &v2){
        Vect va;
        va.x = x + v2.x;
        va.y = y + v2.y;
        va.z = z + v2.z;
    }

    void normalize(){
        double length = sqrt(x*x + y*y + z*z);
        if(length>0){
            x /= length;
            y /= length;
            z /= length;
        }
    }
};

struct Face{
    vector<int> vertex;
    vector<int> vnorm;
};



#endif // GLOBAL_H

