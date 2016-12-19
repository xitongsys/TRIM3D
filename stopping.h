#ifndef STOPPING_H
#define STOPPING_H


class Stopping{//E:keV
public:
    vector<vector<double> > pcoef;
    vector<vector<double> > atomInf;

public:
    double hstop(int Z1, double M1,
                 int Z2, double M2, double E);
    double hestop(int Z1, double M1,
                 int Z2, double M2, double E);
    double histop(int Z1, double M1,
                  int Z2, double M2,
                  double E, double VFERMI, double LFCTR);
    double nstop(int Z1, double M1, int Z2, double M2,
                 double E, double P,
                 double &dE, double &angle1, double &angle2);
public:
    Stopping(string coefFile);
};

#endif // STOPPING_H
