#ifndef QTDATA_H
#define QTDATA_H

#include <vector>
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
    vector<QTIon> ions;
    vector<QTObj> objs;
};


#endif // QTDATA_H
