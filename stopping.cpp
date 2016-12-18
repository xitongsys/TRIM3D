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


double Stopping::histop(int Z1, double M1, int Z2, double M2, double E, double VFERMI, double LFCTR){
    double YRMIN = 0.13, VRMIN = 1.0;
    double V = sqrt(E/25)/VFERMI;
    double VR = V*VFERMI*(1 + 1/(5*V*V));
    if(V < 1){
        VR = (3*VFERMI/4)*(1 + (2*V*V/3) - pow(V,4)/15.0);
    }
    double YR = max(YRMIN, VR/pow(Z1, 0.6667));
    YR = max(YR, VRMIN/pow(Z1, 0.6667));

    double A = -0.803*pow(YR,0.3) + 1.3167*pow(YR, 0.6) + 0.38157*YR + 0.008983*YR*YR;
    double Q = min(1.0, max(0.0, 1.0 - exp(-min(A, 50))));

    double B = (min(0.43, max(0.32, 0.12+0.025*Z1)))/pow(Z1, 0.3333);
    double L0 = (0.8 - Q*(min(1.2, 0.6+Z1/30.0)))/pow(Z1, 0.3333);

    if(Q < 0.2){
        L1 = 0;
    }
    else if(Q < (max(0.0, 0.9 - 0.025*Z1))){
        Q1 = 0.2;
        L1 = B*(Q - 0.2)/abs(max(0.0, 0.9-0.025*Z1) - 0.2000001);
    }
    else if(Q < (max(0.0, 1.0 - 0.025*min(16.0, 1.0*Z1)))){
        L1 = B;
    }

    double L = max(L1, L0*LFCTR);
    ZETA = Q + (1.0/(2.0*VFERMI*VFERMI))*(1.0-Q)*log(1 + pow((4*L*VFERMI/1.919),2));
    A = -pow((7.6 - max(0.0, log(E))),2);
    ZETA = ZETA * (1.0 + (1.0/Z1/Z1)*(0.18 + 0.0015*Z2)*exp(A));

    if(YR <= max(YRMIN, VRMIN/pow(Z1, 0.6667))){
        VRMIN = max(VRMIN, YRMIN*pow(Z1, 0.6667));
        VMIN = 0.5*(VRMIN + sqrt(max(0.0, VRMIN*VRMIN - 0.8*VFERMI*VFERMI)));
        EEE = 25*VMIN*VMIN;
        SP=hstop(Z1,M1,Z2,M2,EEE);
        POWER = 0.5;
        if((Z2==6) || (((Z2==14) || (Z2==32)) && (Z1==19))) POWER = 0.375;
        SE = (SP*pow((ZETA*Z1),2))*pow((E/EEE),POWER);
    }
    else{
        SP = hstop(Z1, M1, Z1, M2, E);
        SE = SP*pow((ZETA*Z1),2);
    }

    SE = SE/(CeV/A);
    return SE;
}




























