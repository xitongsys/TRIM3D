#ifndef STOPPING_H
#define STOPPING_H


class Stopping{
public:
    vector<vector<double> > pcoef;
    vector<vector<double> > atomInf;

public:
    //proton electronic stopping
    double hstop(int Z1, double M1,
                 int Z2, double M2, double E);//E:keV/amu

    //He electronic stopping
    double hestop(int Z1, double M1,
                 int Z2, double M2, double E);//E:keV/amu

    //Heavy ions electronic stopping
    double histop(int Z1, double M1,
                  int Z2, double M2,
                  double E, double VFERMI, double LFCTR);//E:keV/amu

    //nuclear stopping
    double nstop(int Z1, double M1, int Z2, double M2,
                 double E0KEV, double P,
                 double &dE, double &angle1, double &angle2);//E0KEV: keV, dE:eV
public:
    Stopping(string coefFile);
};

#endif // STOPPING_H


