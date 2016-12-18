#include "sample.h"
#include "object3d.h"
#include "global.h"

Sample::Sample(){
    xmin = INT_MAX; xmax = INT_MIN;
    ymin = INT_MAX; ymax = INT_MIN;
    zmin = INT_MAX; zmax = INT_MIN;
}

void Sample::loadInput(string fname){
    ifstream fi(fname);
    char buff[1024];
    while(!fi.eof()){
        memset(buff, 0, sizeof(buff));
        fi.getline(buff, 1024);
        string str(buff);
        stringstream ss(str);
        string head; ss>>head;

        if(head=="obj"){
            int ln = objs.size();
            objs.push_back(Object3D());
            string fname; int eleNum=0;
            ss>>fname; ss>>eleNum;
            string eName; int Z;
            double mass, density, fraction, disE;
            for(int i=0; i<eleNum; i++){
                fi.getline(buff, 1024);
                str=string(buff);
                ss.clear(); ss.str(""); ss<<str;
                ss>>eName>>Z>>mass>>density>>fraction>>disE;
                objs[ln].loadObj(fname);
                Atom atom(eName, Z, mass, 0,0,0, 0,0,0, TARGET);
                objs[ln].addElement(atom, fraction, density, disE);
            }

            xmin=min(xmin, objs[ln].xmin);
            xmax=max(xmax, objs[ln].xmax);
            ymin=min(ymin, objs[ln].ymin);
            ymax=max(ymax, objs[ln].ymax);
            zmin=min(zmin, objs[ln].zmin);
            zmax=max(zmax, objs[ln].zmax);

        }
        else if(head=="ion"){
            int kind; ss>>kind;
            for(int i=0; i<kind; i++){
                fi.getline(buff,1024);
                str=string(buff);
                ss.clear(); ss.str(""); ss<<str;
                string name;
                int Z,num; double mass,x,y,z,vx,vy,vz;
                ss>>name>>Z>>mass>>num>>x>>y>>z>>vx>>vy>>vz;
                int ln = ions.size();
                Atom atom(name, Z, mass, x, y, z, vx, vy, vz, INCIDENT);
                ions.push_back(atom);
                ionNum.push_back(num);
            }
        }
    }
    fi.close();
}
