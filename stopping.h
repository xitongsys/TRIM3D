#ifndef STOPPING_H
#define STOPPING_H


class Stopping{
public:
    double hstop(int Z1, double M1,
                 int Z2, double M2, double E);
    double hestop(int Z1, double M1,
                 int Z2, double M2, double E);
    vector<vector<double> > pcoef;

public:
    Stopping(string coefFile);
};

#endif // STOPPING_H
