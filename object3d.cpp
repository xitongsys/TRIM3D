#include "object3d.h"

Object3D::Object3D() {
    xmin = INT_MAX; xmax = INT_MIN;
    ymin = INT_MAX; ymax = INT_MIN;
    zmin = INT_MAX; zmax = INT_MIN;
}

void Object3D::loadObj(string fname){
    ifstream fin(fname);

    while(!fin.eof()){
        char buff[1024];
        memset(buff, 0, sizeof(buff));
        fin.getline(buff, 1024);

        string str(buff);
        stringstream ss(str);
        string head;
        ss>>head;
        if(head.size()<1 || head[0]=='#') continue;
        if(head=="o"){
            ss>>name;
        }
        else if(head=="v"){
            double x,y,z;
            ss>>x>>y>>z;
            xmin=min(xmin, x); xmax=max(xmax, x);
            ymin=min(ymin, y); ymax=max(ymax, y);
            zmin=min(zmin, z); zmax=max(zmax, z);
            points.push_back(Vect(x,y,z));
        }
        else if(head=="vn"){
            double x,y,z;
            ss>>x>>y>>z;

            vnorms.push_back(Vect(x,y,z));
        }
        else if(head=="f"){
            int lf = faces.size();
            faces.push_back(Face());
            while(!ss.eof()){
                string tmp;
                ss>>tmp;

                int lt = tmp.size();
                int i=0,j=0;
                vector<string> vs;
                while(i<lt && j<lt){
                    while(j<lt && tmp[j]!='/')j++;
                    vs.push_back(tmp.substr(i,j-i));
                    j++; i=j;
                }


                if(vs.size()>=1){
                    stringstream s2i(vs[0]);
                    int ind; s2i>>ind;
                    faces[lf].vertex.push_back(ind-1);
                    if(vs.size()>=3){
                        s2i.clear(); s2i.str("");s2i<<vs[2]; s2i>>ind;
                        faces[lf].vnorm.push_back(ind-1);
                    }
                    else{
                        faces[lf].vnorm.push_back(-1);
                    }
                }
            }

            if(faces[lf].vertex.size()>=3){
                int vi0 = faces[lf].vertex[0];
                int vi1 = faces[lf].vertex[1];
                int vi2 = faces[lf].vertex[2];
                Vect v01 = points[vi1] - points[vi0];
                Vect v12 = points[vi2] - points[vi1];
                Vect vnorm = v01^v12;
                vnorm.normalize();
                int lv = vnorms.size();
                vnorms.push_back(vnorm);

                for(int i=0; i<faces[lf].vertex.size(); i++){
                    if(faces[lf].vnorm[i]<0){
                        faces[lf].vnorm[i]=lv;
                    }
                }
            }


        }

    }
    fin.close();
}


void Object3D::addElement(Atom type, double frac, double dens, double disE){
    elements.push_back(type);
    fraction.push_back(frac);
    density.push_back(dens);
    disEnergy.push_back(disE);

    double sum=0;
    for(int i=0; i<fraction.size(); i++){
        sum += fraction[i];
    }
    ZAve=0; massAve=0; densityAve=0;
    for(int i=0; i<fraction.size(); i++){
        fraction[i] /= sum;
        ZAve += elements[i].Z * fraction[i];
        massAve += element[i].mass * fraction[i];
        densityAve += density[i];
    }
}


bool Object3D::ifin(Vect& pos){
    double x=pos.x, y=pos.y, z=pos.z;
    for(int i=0; i<points.size(); i++){
        double px = points[i].x, py = points[i].y, pz = points[i].z;
        if(px*x + py*y + pz*z >= 0) return false;
    }
    return true;
}


Vect Object3D::lineInteraction(Vect &pos, Vect &direct){
    Vect ans(INT_MAX, INT_MAX, INT_MAX);
    double x0 = pos.x, y0 = pos.y, z0 = pos.z;
    double vx = direct.x, vy = direct.y, vz = direct.z;
    double x,y,z;

    for(int i=0; i<faces.size(); i++){
        double xp = points[faces[i].vertex[0]].x;
        double yp = points[faces[i].vertex[0]].y;
        double zp = points[faces[i].vertex[0]].z;
        double nx = vnorms[faces[i].vnorm[0]].x;
        double ny = vnorms[faces[i].vnorm[0]].y;
        double nz = vnorms[faces[i].vnorm[0]].z;

        if(vx*nx+vy*ny+vz*nz==0) continue;

        double k = ((xp - x0)*nx + (yp -y0)*ny + (zp -z0)*nz ) / (vx*nx + vy*ny + vz*nz);
        Vect tmp(k*vx+x0, k*vy+y0, k*vz+z0);

        if(k>0 && pos.dis(tmp)<pos.dis(ans)){
            ans = tmp;
        }
    }

    return ans;
}














