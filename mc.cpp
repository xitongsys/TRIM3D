#include "mc.h"


MC::MC(string inFile, string sFile):Sample(inFile),Stopping(sFile){}

double MC::randMC(){
    int MOD=1000000;
    double rnd = rand()%MOD;
    rnd /= MOD;
    return rnd;
}

void MC::run(){
    for(int i=0; i<ions.size(); i++){
        for(int j=0; j<ionNum[i]; j++){
            int rc = record.size();
            record.push_back(vector<Atom>(1, ions[i]));
            int k=0;
            while(k<record[rc].size()){
                while(true){
                    Vect pos = record[rc][k].pos;
                    Vect dir = record[rc][k].direct;

                    int oi=0;
                    for(oi=0; oi<objs.size(); oi++){
                        if(objs[oi].ifin(pos)){
                            break;
                        }
                    }

                    if(oi < objs.size()){
                        int eNum = objs[oi].elements.size();
                        int ei=0;
                        double rnd = randMC();
                        double sum=0;
                        while(sum<rnd){
                            sum += objs[oi].fraction[ei];
                            ei++;
                        }
                        ei--;

                        double TMIN=5;
                        double Z1=record[rc][k].Z, Z2=objs[oi].elements[ei].Z, Z2Ave=objs[oi].ZAve;
                        double M1=record[rc][k].mass, M2=objs[oi].elements[ei].mass, M2Ave=objs[oi].massAve;
                        double MUAve=M1/M2Ave;
                        double A = 0.5292*0.8853/(pow(Z1,0.23) + pow(Z2Ave,0.23));
                        double F = A*M2Ave/(Z1*Z2Ave*14.4*(M1+M2Ave));
                        double E = record[rc][k].energy;
                        double EPSDG=TMIN*F*(1 + MUAve)*(1+MUAve)/(4*MUAve);
                        double EPS=E*F;
                        double EEG = sqrt(EPS*EPSDG);
                        double PMAX = A/(EEG + sqrt(EEG) + 0.125*pow(EEG,0.1));
                        double P = PMAX*sqrt(randMC());
                        double ARHO=objs[oi].densityAve;
                        double LS = 1.0/(CPI*PMAX*PMAX*ARHO);

                        double dNE,angle1,angle2;
                        nstop(Z1,M1,Z2,M2,E,P,dNE,angle1,angle2);
                        double dEE;
                        if(Z1==1){dEE=hstop(Z1,M1,Z2,M2,E)*LS;}
                        else if(Z1==2){dEE=hestop(Z1,M1,Z2,M2,E)*LS;}
                        else{
                            dEE=histop(Z1,M1,Z2,M2,E,atomInf[Z2][6],atomInf[Z1][7]);
                        }

                        Atom ion = record[rc][k];
                        double a1,a2;
                        ion.direct.getAngle(a1,a2);

                        if(dNE > objs[oi].disEnergy[ei]){
                            double recoilEnergy = dNE - objs[oi].disEnergy[ei];
                            Atom recoil(objs[oi].elements[ei].name, Z2, M2, ion.pos, Vect(0,0,1), recoilEnergy, RECOIL);
                            recoil.direct.Ry(angle2);
                            recoil.direct.Ry(a2);
                            recoil.direct.Rz(a1);
                            record[rc].push_back(recoil);
                        }

                        Vect tmp(0,0,1);
                        tmp.Ry(-angle1);
                        tmp.Ry(a2); tmp.Rz(a1);
                        record[rc][k].direct = tmp;
                        record[rc][k].energy = (E - dEE - dNE);
                        record[rc][k].pos.x += LS*record[rc][k].direct.x;
                        record[rc][k].pos.y += LS*record[rc][k].direct.y;
                        record[rc][k].pos.z += LS*record[rc][k].direct.z;

                        double EF=5;
                        if(record[rc][k].energy < EF) k++;

                    }
                    else{
                        Vect nP(INT_MAX, INT_MAX, INT_MAX);
                        for(oi=0; oi<objs.size(); oi++){
                            Vect tmp = objs[oi].lineInteraction(pos, dir);
                            if(pos.dis(nP) > pos.dis(tmp)){
                                nP = tmp;
                            }
                        }
                        if(pos.dis(nP)>=INT_MAX) break;
                        else{
                            record[rc][k].pos = nP;
                        }
                    }


                }

            }

        }
    }

}
