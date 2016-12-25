#ifndef QTDATA_H
#define QTDATA_H

#include <vector>
#include <fstream>
#include <sstream>
#include "mc.h"
using namespace std;

class QTEle{
public:
    string name;
    int Z;
    double M;
    double density;
    double fraction;
    double disE;

    QTEle(string namep, int Zp, double Mp, double densp, double fracp, double disEp){
        name = namep;
        M = Mp;
        Z = Zp;
        density = densp;
        fraction = fracp;
        disE = disEp;
    }
};

class QTObj{
public:
    string objFile;
    vector<QTEle> elements;
    void addEle(QTEle ele){
        elements.push_back(ele);
    }
    QTObj(string fname){
        objFile = fname;
    }

    void delEle(int i){
        int ln = elements.size();
        if(i>=0 && i<=ln-1){
            elements.erase(elements.begin() + i);
        }
    }
};

class QTIon{
public:
    string name;
    int Z;
    double M;
    int number;
    double x,y,z;
    double vx,vy,vz;
    double energy;
    QTIon(string namep, int Zp, double Mp, int nump,
          double xp, double yp, double zp, double vxp, double vyp, double vzp,
          double energyp){
        name = namep;
        Z = Zp; M = Mp; number = nump;
        x = xp; y = yp; z = zp;
        vx = vxp; vy = vyp; vz = vzp;
        energy = energyp;
    }
};

class QTInputData{
public:
    QTInputData(){
        filePath="";
    }
    void clear(){
        ions.clear();
        objs.clear();
        filePath="";
    }

    string filePath;
    vector<QTIon> ions;
    vector<QTObj> objs;

    void loadInput(string path){
        filePath = path;
        ifstream fi(path.c_str());
        if(!fi.is_open()) return;

        char buff[1024];
        while(!fi.eof()){
            memset(buff, 0, sizeof(buff));
            fi.getline(buff, 1024);
            string str(buff);
            stringstream ss(str);
            string head; ss>>head;

            if(head=="obj"){
                int ln = objs.size();
                string fname; int eleNum=0;
                string tmp;
                while(ss>>tmp){
                    fname += tmp;
                    if(*(tmp.end()-1)=='"') break;
                }

                int lf=fname.size();
                fname=fname.substr(1, lf-2);

                ss>>eleNum;
                string eName; int Z;
                double mass, density, fraction, disE;
                objs.push_back(QTObj(fname));

                for(int i=0; i<eleNum; i++){
                    fi.getline(buff, 1024);
                    str=string(buff);
                    ss.clear(); ss.str(""); ss<<str;
                    ss>>eName>>Z>>mass>>density>>fraction>>disE;
                    objs[ln].addEle(QTEle(eName,Z,mass,density,fraction,disE));
                }
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
                    ions.push_back(QTIon(name,Z,mass,num,x,y,z,vx,vy,vz,energy));
                }
            }
        }
        fi.close();
    }

    void saveInput(){
        ofstream of;
        of.open(filePath.c_str());
        if(!of.is_open()) return;

        for(int i=0; i<(int)objs.size(); i++){
            of<<"obj \""<<objs[i].objFile<<"\" "<<objs[i].elements.size()<<endl;
            for(int j=0; j<(int)objs[i].elements.size(); j++){
                QTEle ele=objs[i].elements[j];
                of<<ele.name<<" "<<ele.Z<<" "<<ele.M<<" "<<ele.density<<" "<<ele.fraction<<" "<<ele.disE<<endl;
            }
        }

        of<<"ion "<<ions.size()<<endl;
        for(int i=0; i<(int)ions.size(); i++){
            QTIon ion= ions[i];
            of<<ion.name<<" "<<ion.Z<<" "<<ion.M<<" "<<ion.number<<" "<<ion.x<<" "<<ion.y<<" "<<ion.z<<" "<<ion.vx<<" "<<ion.vy<<" "<<ion.vz<<" "<<ion.energy<<endl;
        }
        of.close();
    }

    void saveExport(string name, MC *pmc){
        ofstream of(name.c_str());
        if(!of.is_open())return;
        of<<"Name Z mass(amu) type(recoil or incident) energy(eV) x y z cosX cosY cosZ"<<endl;
        for(int i=0; i<(int)pmc->record.size(); i++){
            for(int j=0; j<(int)(pmc->record[i]).size();j++){
                Atom atom=pmc->record[i][j];
                double x=atom.pos.x, y=atom.pos.y, z=atom.pos.z;
                double vx=atom.direct.x, vy=atom.direct.y, vz=atom.direct.z;
                of<<atom.name<<" "<<atom.Z<<" "<<atom.mass<<" "<<atom.type<<" "<<atom.energy<<" ";
                of<<x<<" "<<y<<" "<<z<<" "<<vx<<" "<<vy<<" "<<vz<<endl;
            }
        }

        of.close();
    }
};




#endif // QTDATA_H
