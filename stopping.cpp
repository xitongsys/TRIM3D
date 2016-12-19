#include "global.h"
#include "phyconst.h"
#include "stopping.h"

Stopping::Stopping(string coefFile){
    atomInf = vector<vector<double> >(1, vector<double>(9, 0.0));
    pcoef = vector<vector<double> >(1, vector<double>(9,0.0));
    ifstream fin(coefFile);
    for(int i=0; i<92; i++){
        int ln = atomInf.size();
        atomInf.push_back(vector<double>(9,0));
        for(int j=0; j<9; j++){
            fin>>atomInf[ln][j];
        }
    }
    for(int i=0; i<92; i++){
        int ln = pcoef.size();
        pcoef.push_back(vector<double>(9,0));
        for(int j=0; j<9; j++){
            fin>>pcoef[ln][j];
        }
    }

    /*
    for(int i=0; i<=92; i++){
        for(int j=0; j<9; j++){
            cout<<pcoef[i][j]<<" ";
        }
        cout<<endl;
    }
    */


    fin.close();
}


double Stopping::hstop(int Z1, double M1, int Z2, double M2, double E){//E:keV/amu
    double pE0 = 25.0;//keV/amu
    double pE=max(pE0, E);
    double a,b,c,d,e,f,g,h;
    a = pcoef[Z2][1]; b = pcoef[Z2][2]; c = pcoef[Z2][3]; d = pcoef[Z2][4];
    e = pcoef[Z2][5]; f = pcoef[Z2][6]; g = pcoef[Z2][7]; h = pcoef[Z2][8];

    double sL = a*pow(pE, b) + c*pow(pE, d);
    double sH = e/pow(pE, f)*log(g/pE + h*pE);
    double sE = sL*sH/(sL + sH);

    double VELPWR;
    if(E <= pE0){
        VELPWR = (Z2<=6)?0.25:0.45;
        sE = sE*pow(E/pE0, VELPWR);
    }

    return sE;//eV-A2
}

double Stopping::hestop(int Z1, double M1, int Z2, double M2, double E){//E:keV/amu
    double HE0=1; //keV/amu
    double HE=max(HE0,E);
    double B=log(HE);
    double A=0.2865 + 0.1266*B - 0.001429*B*B + 0.02402*B*B*B - 0.01135*pow(B,4) + 0.001475*pow(B,5);
    double HEH = 1 - exp(-min(30.0, A));
    A = (1.0 + (0.007 + 0.00005*Z2)*exp(-pow((7.6-max(0.0, log(HE))),2)));
    HEH = HEH*A*A;
    double SP=hstop(Z1,M1,Z2,M2, HE);
    double SE = SP*HEH*Z1*Z1;

    if(E <= HE0){
        SE = SE * sqrt(E/HE0);
    }
    return SE;//eV-A2
}

double Stopping::histop(int Z1, double M1, int Z2, double M2, double E, double VFERMI, double LFCTR){//E:keV/amu

    double L1,ZETA,VMIN,EEE,SP,POWER, SE, Q1;

    double YRMIN = 0.13, VRMIN = 1.0;
    double V = sqrt(E/25.0)/VFERMI;
    double VR = V*VFERMI*(1.0 + 1.0/(5*V*V));
    if(V < 1){
        VR = (3*VFERMI/4)*(1 + (2*V*V/3) - pow(V,4)/15.0);
    }
    double YR = max(YRMIN, VR/pow(Z1, 0.6667));
    YR = max(YR, VRMIN/pow(Z1, 0.6667));

    double A = -0.803*pow(YR,0.3) + 1.3167*pow(YR, 0.6) + 0.38157*YR + 0.008983*YR*YR;
    double Q = min(1.0, max(0.0, 1.0 - exp(-min(A, 50.0))));

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

    return SE;//eV-A2
}

double Stopping::nstop(int Z1, double M1, int Z2, double M2,
                       double E0KEV, double P,
                       double &dE, double &angle1, double &angle2){//E0KEV: keV

    double R,RR,EX1,EX2,EX3,EX4,V,V1,FR,FR1,Q,ROC,SQE,CC,AA,FF;
    double DELTA, CO, C2, S2, CT, ST, EC, DEN;

    double MU = M2/M1, MY = M1/M2;
    double A = 0.5292-0.8853/(pow(Z1, 0.23) + pow(Z2, 0.23));
    double F = A*M2/(Z1*Z2*14.4*(M1 + M2));
    double E = E0KEV*1000;//eV
    double TMIN = 5;
    double EPS = E * F;
    double EPSDG = TMIN*F*(1 + MU)*(1 + MU)/(4*MU);
    double B = P/A;
    if(EPS<=10){
        R = B;
        RR = -2.7*log(EPS*B);
        if(RR >= B){
            RR = -2.7*log(EPS*RR);
            if(RR >= B){
                R = RR;
            }
        }
        while(true){
            EX1 = 0.18175*exp(-3.1998*R);
            EX2 = 0.50986*exp(-0.94229*R);
            EX3 = 0.28022*exp(-0.4029*R);
            EX4 = 0.028171*exp(-0.20162*R);
            V = (EX1 + EX2 + EX3 + EX4)/R;
            V1 = -(V + 3.1998*EX1 + 0.94229*EX2 + 0.4029*EX3 + 0.20162*EX4)/R;

            FR = B*B/R + V*R/EPS - R;
            FR1 = -B*B/(R*R) + (V + V1*R)/EPS - 1;
            Q = FR/FR1;
            R = R - Q;
            if(abs(Q/R) <= 0.001) break;
        }

        ROC = -2.0*(EPS - V)/V1;
        SQE = sqrt(EPS);

        CC=(0.011615+SQE)/(0.0071222+SQE);
        AA=2.0*EPS*(1.0+(0.99229/SQE))*pow(B,CC);
        FF=(sqrt(AA*AA+1.0)-AA)*((9.3066+EPS)/(14.813+EPS));

        DELTA=(R-B)*AA*FF/(FF+1.0);
        CO=(B+DELTA+ROC)/(R+ROC);
        C2=CO*CO;
        S2=1.0-C2;
        CT=2.0*C2-1.0;
        ST=sqrt(1.0-CT*CT);
    }
    else{
        S2=1.0/(1.0+(1.0+B*(1.0+B))*(2.0*EPS*B)*(2.0*EPS*B));
        C2=1.0-S2;
        CT=2.0*C2-1.0;
        ST=sqrt(1.0-CT*CT);
    }

    EC = 4*MY/pow((1+MY),2);
    DEN = EC*S2*E;

    dE = DEN;//eV
    angle1 = atan(ST/(CT + MY)); //scattering angle of incident ion in Lab system
    if(angle1<0) angle1 += CPI;
    double T = DEN;
    double Tm = 4*M1*M2/pow((M1+M2),2)*E;
    angle2 = acos(sqrt(T/Tm)); //scattering angle of target atom in Lab system

    return dE; //eV
}




























