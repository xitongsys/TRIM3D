#ifndef GLOBAL_H
#define GLOBAL_H

#include <bits/stdc++.h>
#include <fstream>
#include "phyconst.h"

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
        return va;
    }

    double dis(Vect& v2){
        return sqrt((x-v2.x)*(x-v2.x) + (y-v2.y)*(y-v2.y) + (z-v2.z)*(z-v2.z));
    }

    void getAngle(double &angle1, double &angle2){
        double l = sqrt(x*x + y*y);
        angle1 = acos(x/l);
        if(x*y<0){
            angle1 += CPI;

        }
        l = sqrt(x*x + y*y + z*z);
        angle2 = acos(z/l);
    }

    void Rz(double a){
        double xx=x, yy=y, zz=z;
        x = xx*cos(a) - yy*sin(a);
        y = xx*sin(a) + yy*cos(a);
        z = zz;
    }

    void Rx(double a){
        double xx=x, yy=y, zz=z;
        x = xx;
        y = yy*cos(a) - zz*sin(a);
        z = yy*sin(a) + zz*cos(a);
    }

    void Ry(double a){
        double xx=x, yy=y, zz=z;
        x = xx*cos(a) + zz*sin(a);
        y = yy;
        z = -xx*sin(a) + zz*cos(a);

    }

    void rotate(Vect &pos, Vect &direct, double angle){
        Vect tmp(x-pos.x,y-pos.y,z-pos.z);
        double a1, a2;
        direct.getAngle(a1,a2);
        tmp.Rz(-a1);
        tmp.Ry(-a2);
        tmp.Rz(angle);
        tmp.Ry(a2);
        tmp.Rz(a1);
        tmp.x += pos.x;
        tmp.y += pos.y;
        tmp.z += pos.z;
        x=tmp.x; y=tmp.y; z=tmp.z;
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

