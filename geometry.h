#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "global.h"
#include <iostream>
using namespace std;

class Geometry{
public:
    void drawSphere(vector<double> &mem, Vect &pos, Color4f &col, double R, int slice){
        mem.clear();
        double da1 = CPI/slice, da2 = 2*CPI/slice;
        for(int i=1; i<=slice; i++){
            for(int j=0; j<slice; j++){
                double x,y,z,vx,vy,vz,a1,a2; int ii,jj;

                ii = i; jj = j;
                a1 = da1*ii;
                a2 = da2*jj;
                z = R*cos(a1);
                x = R*sin(a1)*cos(a2);
                y = R*sin(a1)*sin(a2);
                vx = x/R; vy=y/R; vz=z/R;
                x+=pos.x; y+=pos.y; z+=pos.z;
                mem.push_back(x); mem.push_back(y); mem.push_back(z);
                mem.push_back(vx); mem.push_back(vy); mem.push_back(vz);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                ii = i-1; jj = j-1; if(jj<0)jj+=slice;
                a1 = da1*ii;
                a2 = da2*jj;
                z = R*cos(a1);
                x = R*sin(a1)*cos(a2);
                y = R*sin(a1)*sin(a2);
                vx = x/R; vy=y/R; vz=z/R;
                x+=pos.x; y+=pos.y; z+=pos.z;
                mem.push_back(x); mem.push_back(y); mem.push_back(z);
                mem.push_back(vx); mem.push_back(vy); mem.push_back(vz);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                ii = i; jj = j-1; if(jj<0)jj+=slice;
                a1 = da1*ii;
                a2 = da2*jj;
                z = R*cos(a1);
                x = R*sin(a1)*cos(a2);
                y = R*sin(a1)*sin(a2);
                vx = x/R; vy=y/R; vz=z/R;
                x+=pos.x; y+=pos.y; z+=pos.z;
                mem.push_back(x); mem.push_back(y); mem.push_back(z);
                mem.push_back(vx); mem.push_back(vy); mem.push_back(vz);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                ii = i; jj = j;
                a1 = da1*ii;
                a2 = da2*jj;
                z = R*cos(a1);
                x = R*sin(a1)*cos(a2);
                y = R*sin(a1)*sin(a2);
                vx = x/R; vy=y/R; vz=z/R;
                x+=pos.x; y+=pos.y; z+=pos.z;
                mem.push_back(x); mem.push_back(y); mem.push_back(z);
                mem.push_back(vx); mem.push_back(vy); mem.push_back(vz);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                ii = i-1; jj = j;
                a1 = da1*ii;
                a2 = da2*jj;
                z = R*cos(a1);
                x = R*sin(a1)*cos(a2);
                y = R*sin(a1)*sin(a2);
                vx = x/R; vy=y/R; vz=z/R;
                x+=pos.x; y+=pos.y; z+=pos.z;
                mem.push_back(x); mem.push_back(y); mem.push_back(z);
                mem.push_back(vx); mem.push_back(vy); mem.push_back(vz);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                ii = i-1; jj = j-1; if(jj<0)jj+=slice;
                a1 = da1*ii;
                a2 = da2*jj;
                z = R*cos(a1);
                x = R*sin(a1)*cos(a2);
                y = R*sin(a1)*sin(a2);
                vx = x/R; vy=y/R; vz=z/R;
                x+=pos.x; y+=pos.y; z+=pos.z;
                mem.push_back(x); mem.push_back(y); mem.push_back(z);
                mem.push_back(vx); mem.push_back(vy); mem.push_back(vz);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);
            }
        }
    }

};








#endif // GEOMETRY_H
