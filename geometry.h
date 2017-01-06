#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "global.h"
#include <iostream>
using namespace std;

class Geometry{
public:
    void drawSphere(vector<double> &mem, Vect &pos, Color4f &col, double R, int slice){
        //mem.clear();
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

    void drawCone(vector<double> &mem, Vect &pos, Color4f &col, double R, double H, int slice, double Ry, double Rz){
        //mem.clear();
        double da=2*CPI/slice;
        for(int i=1; i<=slice; i++){
            double a1=(i-1)*da, a2=i*da;
            Vect v1(cos(a1)*R, sin(a1)*R, 0);
            Vect v2(cos(a2)*R, sin(a2)*R, 0);
            Vect v3(0, 0, H);
            Vect v12=v2-v1, v13=v3-v1;
            Vect vn=v12^v13; vn.normalize();
            v1.Ry(Ry); v1.Rz(Rz);v2.Ry(Ry); v2.Rz(Rz);v3.Ry(Ry); v3.Rz(Rz);
            vn.Ry(Ry); vn.Rz(Rz);

            mem.push_back(v1.x+pos.x); mem.push_back(v1.y+pos.y); mem.push_back(v1.z+pos.z);
            mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
            mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

            mem.push_back(v2.x+pos.x); mem.push_back(v2.y+pos.y); mem.push_back(v2.z+pos.z);
            mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
            mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

            mem.push_back(v3.x+pos.x); mem.push_back(v3.y+pos.y); mem.push_back(v3.z+pos.z);
            mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
            mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

        }

        for(int i=1; i<=slice; i++){
            double a2=(i-1)*da, a1=i*da;
            Vect v1(cos(a1)*R, sin(a1)*R, 0);
            Vect v2(cos(a2)*R, sin(a2)*R, 0);
            Vect v3(0, 0, 0);
            Vect v12=v2-v1, v13=v3-v1;
            Vect vn=v12^v13; vn.normalize();
            v1.Ry(Ry); v1.Rz(Rz);v2.Ry(Ry); v2.Rz(Rz);v3.Ry(Ry); v3.Rz(Rz);
            vn.Ry(Ry); vn.Rz(Rz);

            mem.push_back(v1.x+pos.x); mem.push_back(v1.y+pos.y); mem.push_back(v1.z+pos.z);
            mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
            mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

            mem.push_back(v2.x+pos.x); mem.push_back(v2.y+pos.y); mem.push_back(v2.z+pos.z);
            mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
            mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

            mem.push_back(v3.x+pos.x); mem.push_back(v3.y+pos.y); mem.push_back(v3.z+pos.z);
            mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
            mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

        }

    }

    void drawCylinder(vector<double> &mem, Vect &pos, Color4f &col, double R, double H, int slice, double Ry, double Rz){
            //mem.clear();
            double da=2*CPI/slice;
            for(int i=1; i<=slice; i++){
                double a1=(i-1)*da, a2=i*da;
                Vect v1(cos(a1)*R, sin(a1)*R, 0);
                Vect v2(cos(a2)*R, sin(a2)*R, 0);
                Vect v3(cos(a1)*R, sin(a1)*R, H);
                Vect v4(cos(a2)*R, sin(a2)*R, H);

                Vect v12=v2-v1, v13=v3-v1;
                Vect vn=v12^v13; vn.normalize();
                v1.Ry(Ry); v1.Rz(Rz);v2.Ry(Ry); v2.Rz(Rz);v3.Ry(Ry); v3.Rz(Rz);
                vn.Ry(Ry); vn.Rz(Rz);

                mem.push_back(v1.x+pos.x); mem.push_back(v1.y+pos.y); mem.push_back(v1.z+pos.z);
                mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                mem.push_back(v2.x+pos.x); mem.push_back(v2.y+pos.y); mem.push_back(v2.z+pos.z);
                mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                mem.push_back(v3.x+pos.x); mem.push_back(v3.y+pos.y); mem.push_back(v3.z+pos.z);
                mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                v4.Ry(Ry); v4.Rz(Rz);
                mem.push_back(v3.x+pos.x); mem.push_back(v3.y+pos.y); mem.push_back(v3.z+pos.z);
                mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                mem.push_back(v2.x+pos.x); mem.push_back(v2.y+pos.y); mem.push_back(v2.z+pos.z);
                mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                mem.push_back(v4.x+pos.x); mem.push_back(v4.y+pos.y); mem.push_back(v4.z+pos.z);
                mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);




            }

            for(int i=1; i<=slice; i++){
                double a2=(i-1)*da, a1=i*da;
                Vect v1(cos(a1)*R, sin(a1)*R, 0);
                Vect v2(cos(a2)*R, sin(a2)*R, 0);
                Vect v3(0, 0, 0);
                Vect v12=v2-v1, v13=v3-v1;
                Vect vn=v12^v13; vn.normalize();
                v1.Ry(Ry); v1.Rz(Rz);v2.Ry(Ry); v2.Rz(Rz);v3.Ry(Ry); v3.Rz(Rz);
                vn.Ry(Ry); vn.Rz(Rz);

                mem.push_back(v1.x+pos.x); mem.push_back(v1.y+pos.y); mem.push_back(v1.z+pos.z);
                mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                mem.push_back(v2.x+pos.x); mem.push_back(v2.y+pos.y); mem.push_back(v2.z+pos.z);
                mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                mem.push_back(v3.x+pos.x); mem.push_back(v3.y+pos.y); mem.push_back(v3.z+pos.z);
                mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

            }

            for(int i=1; i<=slice; i++){
                double a1=(i-1)*da, a2=i*da;
                Vect v1(cos(a1)*R, sin(a1)*R, H);
                Vect v2(cos(a2)*R, sin(a2)*R, H);
                Vect v3(0, 0, H);
                Vect v12=v2-v1, v13=v3-v1;
                Vect vn=v12^v13; vn.normalize();
                v1.Ry(Ry); v1.Rz(Rz);v2.Ry(Ry); v2.Rz(Rz);v3.Ry(Ry); v3.Rz(Rz);
                vn.Ry(Ry); vn.Rz(Rz);

                mem.push_back(v1.x+pos.x); mem.push_back(v1.y+pos.y); mem.push_back(v1.z+pos.z);
                mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                mem.push_back(v2.x+pos.x); mem.push_back(v2.y+pos.y); mem.push_back(v2.z+pos.z);
                mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

                mem.push_back(v3.x+pos.x); mem.push_back(v3.y+pos.y); mem.push_back(v3.z+pos.z);
                mem.push_back(vn.x); mem.push_back(vn.y); mem.push_back(vn.z);
                mem.push_back(col.r); mem.push_back(col.g); mem.push_back(col.b); mem.push_back(col.a);

            }

    }

    void drawAxes3D(vector<double> &mem, double size){
        Vect pos; Color4f col; double R, H, Ry, Rz;
        int slice=20;
        //x
        pos=Vect(2*size, 0, 0); col=Color4f(1, 0, 0, 1);
        R=size; H=size; Ry=CPI/2; Rz=0;
        drawCone(mem, pos,col, R, H, slice, Ry, Rz);
        pos=Vect(0, 0, 0); col=Color4f(1, 0, 0, 1);
        R=size/2; H=size*2; Ry=CPI/2; Rz=0;
        drawCylinder(mem, pos, col, R, H, slice, Ry, Rz);

        //y
        pos=Vect(0, 2*size, 0); col=Color4f(0, 1, 0, 1);
        R=size; H=size; Ry=CPI/2; Rz=CPI/2;
        drawCone(mem, pos,col, R, H, slice, Ry, Rz);
        pos=Vect(0, 0, 0); col=Color4f(0, 1, 0, 1);
        R=size/2; H=2*size; Ry=CPI/2; Rz=CPI/2;
        drawCylinder(mem, pos, col, R, H, slice, Ry, Rz);


        //z
        pos=Vect(0,0,size*2); col=Color4f(0,0,1,1);
        R=size; H=size; Ry=0; Rz=0;
        drawCone(mem, pos,col, R, H, slice, Ry, Rz);
        pos=Vect(0,0,0); col=Color4f(0,0,1,1);
        R=size/2; H=size*2; Ry=0; Rz=0;
        drawCylinder(mem, pos, col, R, H, slice, Ry, Rz);



    }

};








#endif // GEOMETRY_H
