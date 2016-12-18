#include "global.h"
#include "stopping.h"

Stopping::Stopping(string coefFile){
    pcoef = new vector<vector<double> >(1, new vector<double>(9));
    ifstream fin(coefFile);
    while(!fin.eof()){
        int ln = pcoef.size();
        pcoef.push_back(new vector<double>(9,0));
        for(int i=0; i<9; i++){
            fin>>pcoef[ln][i];
        }
    }
    fin.close();
}


double Stopping::hstop(int Z2, double M2, double E){
    double pE0 = 25.0;
    double pE=max(pE0, E);
    double a,b,c,d,e,f,g;
    a = pcoef[Z2][1]; b = pcoef[Z2][2]; c = pcoef[Z2][3]; d = pcoef[Z2][4];
    e = pcoef[Z2][5]; f = pcoef[Z2][6]; g = pcoef[Z2][7]; h = pcoef[Z2][8];

    double sL = a*pow(pE, b) + c*pow(pE, d);
    double sH = e/pow(pE, f)*log(g/pE + h*pE);
    double sE = sL*sH/(sL + sH);

    if(E <= pE0){
        VELPWR = (Z2<=6)?0.25:0.45;
        sE = sE*pow(E/pE0, VELPWR);
    }

    sE=sE/(CeV/CA);
    return sE;
}

double Stopping::hestop(int Z1, double M1, int Z2, double M2, double E){
    double HE0=1;
    double HE=max(HE0,E);
    double B=log(HE);
    double A=0.2865 + 0.1266*B - 0.001429*B*B + 0.02402*B*B*B - 0.01135*pow(B,4) + 0.001475*pow(B,5);
    double HEH = 1 - exp(-min(30.0, A));
    A = (1.0 + (0.007 + 0.00005*Z2)*exp(-pow((7.6-max(0, log(HE))),2)));
    HEH = HEH*A*A;
    double SP=hstop(Z1,M1,Z2,M2,E);
    double SE = SP*HEH*Z1*Z1;

    if(E <= HE0){
        SE = SE * sqrt(E/HE0);
    }

    SE = SE/(CeV/CA);
    return SE;
}
