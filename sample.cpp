#include "sample.h"
#include "object3d.h"
#include "global.h"
#include <climits>
#include <memory.h>
#include <sstream>

Sample::Sample(){
    xmin = INT_MAX; xmax = INT_MIN;
    ymin = INT_MAX; ymax = INT_MIN;
    zmin = INT_MAX; zmax = INT_MIN;
}

Sample::Sample(string inputFile){
    xmin = INT_MAX; xmax = INT_MIN;
    ymin = INT_MAX; ymax = INT_MIN;
    zmin = INT_MAX; zmax = INT_MIN;
    loadInput(inputFile);
}

void Sample::loadInput(string fname){
    ifstream fi(fname.c_str());
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
            objs[ln].loadObj(fname);

            for(int i=0; i<eleNum; i++){
                fi.getline(buff, 1024);
                str=string(buff);
                ss.clear(); ss.str(""); ss<<str;
                ss>>eName>>Z>>mass>>density>>fraction>>disE;
                Atom atom(eName, Z, mass, Vect(0,0,0), Vect(0,0,0), 0, TARGET);
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
                int Z,num; double mass,x,y,z,vx,vy,vz,energy;
                ss>>name>>Z>>mass>>num>>x>>y>>z>>vx>>vy>>vz>>energy;
                int ln = ions.size();
                Atom atom(name, Z, mass, Vect(x, y, z), Vect(vx, vy, vz), energy, INCIDENT);
                ions.push_back(atom);
                ionNum.push_back(num);
            }
        }
    }
    fi.close();

    /*
    for(int i=0; i<objs.size(); i++){
        for(int j=0; j<objs[i].elements.size(); j++){
            cout<<objs[i].elements[j].name<<" "<<objs[i].density[j]<<" "<<objs[i].fraction[j]<<" "<<objs[i].disEnergy[j]<<endl;
        }
    }
    for(int i=0; i<ions.size(); i++){
        cout<<ions[i].name<<" "<<" "<<ions[i].energy<<" "<<ionNum[i]<<endl;
    }
    */

}
