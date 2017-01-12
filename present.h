#ifndef PRESENT_H
#define PRESENT_H

#include "global.h"
#include "atom.h"
#include <vector>
#include <map>
#include <stack>
#include <sstream>

using namespace std;



class Present{
public:
    string cmd;
    double R;
    int slice;
    Color4f col;
    vector<string> cmdv;
    map<string, int> order;

    //for plotWindow
    vector<string> typeName;
    int type;
    //

    void cmdSplit();
    void mid2post(vector<string> &mid, vector<string> &postfix);
    bool calPostfix(vector<string> &postfix);
    bool checkObj(int obj);
    bool check(Atom &atom);

    Present(string cmdd, Color4f coll, double RR, int slicee);


};



#endif // PRESENT_H
