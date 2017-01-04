#ifndef DRAWINFO_H
#define DRAWINFO_H

#include "global.h"
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include "atom.h"
#include <iostream>

using namespace std;




class Present{
public:
    string cmd;
    double R;
    int slice;
    Color4f col;
    vector<string> cmdv;



    Present(string cmdd, Color4f coll, double RR, int slicee){
        cmd = cmdd; col=coll; R=RR; slice=slicee;
        cmdSplit();
    }

    void cmdSplit(){
        cmdv.clear();
        cmdv.push_back("(");
        int i=0, j=0;
        int ln=cmd.size();
        while(i<ln && j<ln){
            while(i<ln && cmd[i]==' ')i++;
            j=i;
            while(j<ln && cmd[j]!=' ')j++;
            string tmp=cmd.substr(i, j-i);
            if(tmp.size()>0) cmdv.push_back(tmp);
            i=j;
        }
        cmdv.push_back(")");
    }


    bool check(Atom &atom){
        vector<string> cmdtmp=cmdv;

        int ln=cmdv.size();
        for(int i=0; i<ln; i++){
            stringstream ss;
            if(cmdtmp[i]=="x"){ss<<atom.pos.x;}
            else if(cmdtmp[i]=="y"){ss<<atom.pos.y;}
            else if(cmdtmp[i]=="z"){ss<<atom.pos.z;}
            else if(cmdtmp[i]=="cosX"){ss<<atom.direct.x;}
            else if(cmdtmp[i]=="cosY"){ss<<atom.direct.y;}
            else if(cmdtmp[i]=="cosZ"){ss<<atom.direct.z;}
            else if(cmdtmp[i]=="M"){ss<<atom.mass;}
            else if(cmdtmp[i]=="Z"){ss<<atom.Z;}
            else if(cmdtmp[i]=="T"){ss<<atom.type;}
            else if(cmdtmp[i]=="E"){ss<<atom.energy;}
            else if(cmdtmp[i]=="all") return true;
            ss>>cmdtmp[i];
        }

        stack<string> sk;
        vector<string> postfix;
        ln=cmdtmp.size();
        for(int i=0; i<ln; i++){
            string str=cmdtmp[i];
            if(str=="||" || str=="&&" || str=="==" || str==">" || str=="<" || str==">=" || str=="<=" || str=="!="){
                while(!sk.empty() && sk.top()!="("){
                    postfix.push_back(sk.top());
                    sk.pop();
                }
                sk.push(str);
            }
            else if(str=="("){
                sk.push(str);
            }
            else if(str==")"){
                while(!sk.empty() && sk.top()!="("){
                    postfix.push_back(sk.top());
                    sk.pop();
                }
                if(sk.empty() || sk.top()!="(") return false;
                sk.pop();
            }
            else{
                postfix.push_back(str);
            }
        }

        if(!sk.empty()) return false;

        stack<double> skcal;

        ln=postfix.size();
        for(int i=0; i<ln; i++){
            string str=postfix[i];
            double v1,v2;
            if(str=="||"){
                v2=skcal.top(); skcal.pop();
                v1=skcal.top(); skcal.pop();
                skcal.push(v1 || v2);
            }
            else if(str=="&&"){
                v2=skcal.top(); skcal.pop();
                v1=skcal.top(); skcal.pop();
                skcal.push(v1 && v2);

            }
            else if(str=="=="){
                v2=skcal.top(); skcal.pop();
                v1=skcal.top(); skcal.pop();
                skcal.push(v1 == v2);

            }
            else if(str==">"){
                v2=skcal.top(); skcal.pop();
                v1=skcal.top(); skcal.pop();
                skcal.push(v1 > v2);

            }
            else if(str=="<"){
                v2=skcal.top(); skcal.pop();
                v1=skcal.top(); skcal.pop();
                skcal.push(v1 < v2);

            }
            else if(str==">="){
                v2=skcal.top(); skcal.pop();
                v1=skcal.top(); skcal.pop();
                skcal.push(v1 >= v2);

            }
            else if(str=="<="){
                v2=skcal.top(); skcal.pop();
                v1=skcal.top(); skcal.pop();
                skcal.push(v1 <= v2);

            }
            else if(str=="!="){
                v2=skcal.top(); skcal.pop();
                v1=skcal.top(); skcal.pop();
                skcal.push(v1 != v2);

            }
            else{
                stringstream ss; ss<<str;
                double val; ss>>val;
                skcal.push(val);
            }
        }

        return skcal.top();

    }

};

class DrawInfo{
public:
    vector<Present> pres;

    Color4f bgColor;
    Color4f objColor;

    double angleX, angleY, angleZ;
    double transX, transY, transZ;
    float MR[16];

    double randTmp(){
        int MOD=1000;
        double rnd = rand()%MOD;
        rnd /= MOD;
        return rnd;
    }

    DrawInfo(){

        bgColor=Color4f(0.8, 0.8, 0.8, 1.0);
        objColor=Color4f(0.1, 0.8, 0.8, 0.5);
        pres.push_back(Present("all",Color4f(randTmp(), randTmp(), randTmp(),1.0),2.0, 4));

        angleX = 0; angleY = 0; angleZ=0;
        transX = 0; transY = 0; transZ = 0;
        memset(MR, 0, sizeof(MR));
        MR[0]=1; MR[5]=1; MR[10]=1; MR[15]=1;

    }

};

#endif // DRAWINFO_H







