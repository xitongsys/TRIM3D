#ifndef QTDATA_H
#define QTDATA_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "mc.h"
#include <QDir>

using namespace std;

class QTEle{
public:
    string name;
    int Z;
    double M;
    double fraction;
    double disE;


    QTEle(string namep, int Zp, double Mp, double fracp, double disEp){
        name = namep;
        M = Mp;
        Z = Zp;
        fraction = fracp;
        disE = disEp;
    }
};

class QTObj{
public:
    string objFile;
    double objDensity;
    double scale;
    vector<QTEle> elements;
    void addEle(QTEle ele){
        elements.push_back(ele);
    }
    QTObj(string fname, double objDensityy, double scalee){
        objFile = fname;
        objDensity = objDensityy;
        scale = scalee;
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

        QString stmp; stmp = stmp.fromLocal8Bit(path.c_str());
        QDir dir(stmp);
        string abPath=dir.absolutePath().toLocal8Bit().toStdString();
        MyPath mp(abPath);
        stmp = stmp.fromLocal8Bit(mp.path.c_str());

        QDir::setCurrent(stmp);

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

                double objDensity=0, scale=1;
                ss>>eleNum>>objDensity>>scale;

                string eName; int Z;
                double mass, fraction, disE;
                objs.push_back(QTObj(fname,objDensity, scale));

                for(int i=0; i<eleNum; i++){
                    fi.getline(buff, 1024);
                    str=string(buff);
                    ss.clear(); ss.str(""); ss<<str;
                    ss>>eName>>Z>>mass>>fraction>>disE;
                    objs[ln].addEle(QTEle(eName,Z,mass,fraction,disE));
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


        QString stmp; stmp=stmp.fromLocal8Bit(filePath.c_str());
        QDir dir(stmp);
        string abPath=dir.absolutePath().toLocal8Bit().toStdString();
        MyPath mp(abPath);
        stmp = stmp.fromLocal8Bit(mp.path.c_str());
        QDir::setCurrent(stmp);
        QFile scoefFile(":/SCOEF.88");
        scoefFile.copy("SCOEF.88");

        for(int i=0; i<(int)objs.size(); i++){
            MyPath mpobj(objs[i].objFile);
            if(mpobj.path!=mp.path){
                QFile src(objs[i].objFile.c_str());
                string des=mp.path + mpobj.fname;
                src.copy(des.c_str());
            }
            of<<"obj \""<<mpobj.fname<<"\" "<<objs[i].elements.size()<<" "<<objs[i].objDensity<<" "<<objs[i].scale<<endl;
            for(int j=0; j<(int)objs[i].elements.size(); j++){
                QTEle ele=objs[i].elements[j];
                of<<ele.name<<" "<<ele.Z<<" "<<ele.M<<" "<<" "<<ele.fraction<<" "<<ele.disE<<endl;
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
        int tnum = pmc->incidentNum + pmc->recoilNum;
        of<<tnum<<endl;
        of<<"Name x y z Z mass(amu) type(recoil or incident) energy(eV) cosX cosY cosZ"<<endl;
        for(int i=0; i<(int)pmc->record.size(); i++){
            for(int j=0; j<(int)(pmc->record[i]).size();j++){
                Atom atom=pmc->record[i][j];
                double x=atom.pos.x, y=atom.pos.y, z=atom.pos.z;
                double vx=atom.direct.x, vy=atom.direct.y, vz=atom.direct.z;
                of<<atom.name<<" "<<x<<" "<<y<<" "<<z<<" ";
                of<<atom.Z<<" "<<atom.mass<<" "<<atom.type<<" "<<atom.energy<<" ";
                of<<vx<<" "<<vy<<" "<<vz<<endl;
            }
        }

        of.close();
    }
};




#endif // QTDATA_H
