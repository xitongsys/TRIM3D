#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include <fstream>
#include "phyconst.h"
#include <QMutex>

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

    Vect operator=(Vect v2){
        x=v2.x; y=v2.y; z=v2.z;
    }

    double operator*(Vect &v2){
        return x*v2.x + y*v2.y + z*v2.z;
    }

    Vect operator*(double d){
        Vect va;
        va.x = x*d;
        va.y = y*d;
        va.z = z*d;
        return va;
    }

    double dis(Vect& v2){
        return sqrt((x-v2.x)*(x-v2.x) + (y-v2.y)*(y-v2.y) + (z-v2.z)*(z-v2.z));
    }

    void getAngle(double &angle1, double &angle2){
        double l = sqrt(x*x + y*y);
        if(l==0){
            angle1 = 0; angle2=0;
            return;
        }
        angle1 = acos(x/l);
        if(y<0){
            angle1 = 2*CPI - angle1;

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

class MyPath{
public:
    string path, fname;
    MyPath(string abPath){
        int ln = abPath.size();
        int i=ln-1;
        while(i>=0 && abPath[i]!='/')i--;
        path=abPath.substr(0, i+1);
        fname=abPath.substr(i+1, ln - i - 1);
    }
};

extern QMutex mutexLock;

class DataInfo;
extern DataInfo cd;

#endif // GLOBAL_H

