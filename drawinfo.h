#ifndef DRAWINFO_H
#define DRAWINFO_H

#include "global.h"
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include "atom.h"
#include <iostream>
#include <map>
#include <set>

using namespace std;




class Present{
public:
    string cmd;
    double R;
    int slice;
    Color4f col;
    vector<string> cmdv;
    map<string, int> order;


    Present(string cmdd, Color4f coll, double RR, int slicee){
        order["("]=7; order[")"]=7;
        order["*"]=6; order["/"]=6; order["%"]=6;
        order["+"]=5; order["-"]=5;
        order[">"]=4;order["<"]=4;order["<="]=4;order[">="]=4;
        order["=="]=3;order["!="]=3;
        order["&&"]=2;
        order["||"]=1;

        cmd = cmdd; col=coll; R=RR; slice=slicee;
        cmdSplit();
    }

    void cmdSplit(){       
        string cmdtmp;
        int ln = cmd.size();
        for(int i=0; i<ln; i++){
            if(cmd[i]!=' ' && cmd[i]!='\t'){
                cmdtmp.push_back(cmd[i]);
            }
        }
        cmd=cmdtmp;

        cmdv.clear();
        cmdv.push_back("(");

        int i=0, j=0;
        ln=cmd.size();
        cmd += " ";

        string str1,str2,str3;

        while(i<ln && j<ln){
            j=i;
            while(j<ln){
                str1=cmd.substr(j, 1); str2=cmd.substr(j,2);
                if(order.find(str1)!=order.end() || order.find(str2)!=order.end())break;
                j++;
            }
            str3=cmd.substr(i, j-i);
            if(str3.size()>0) cmdv.push_back(str3);

            if(order.find(str2)!=order.end()){
                cmdv.push_back(str2);
                i=j+2;
            }
            else if(order.find(str1)!=order.end()){
                cmdv.push_back(str1);
                i=j+1;
            }
            else{
                i=j;
            }

        }
        cmdv.push_back(")");
    }

    void mid2post(vector<string> &mid, vector<string> &postfix){
        stack<string> sk;

        int ln=mid.size();
        for(int i=0; i<ln; i++){
            string str=mid[i];
            if(order.find(str)!=order.end()){
                if(str=="("){
                    sk.push(str);
                }
                else if(str==")"){
                    while(!sk.empty() && sk.top()!="("){
                        postfix.push_back(sk.top());
                        sk.pop();
                    }
                    if(sk.empty() || sk.top()!="(") return;
                    sk.pop();
                }
                else{
                    while(!sk.empty() && order[sk.top()]>order[str] && sk.top()!="("){
                        postfix.push_back(sk.top());
                        sk.pop();
                    }
                    sk.push(str);
                }

            }

            else{
                postfix.push_back(str);
            }
        }
    }

    bool calPostfix(vector<string> &postfix){
        stack<double> skcal;

        int ln=postfix.size();
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
            else if(str=="+"){
                v2=skcal.top(); skcal.pop();
                v1=skcal.top(); skcal.pop();
                skcal.push(v1 + v2);
            }
            else if(str=="-"){
                v2=skcal.top(); skcal.pop();
                v1=skcal.top(); skcal.pop();
                skcal.push(v1 - v2);
            }
            else if(str=="*"){
                v2=skcal.top(); skcal.pop();
                v1=skcal.top(); skcal.pop();
                skcal.push(v1 * v2);
            }
            else if(str=="/"){
                v2=skcal.top(); skcal.pop();
                v1=skcal.top(); skcal.pop();
                skcal.push(v1 / v2);
            }

            else{
                stringstream ss; ss<<str;
                double val;
                if(!(ss>>val))return false;
                skcal.push(val);
            }
        }

        if(skcal.empty()) return false;
        return skcal.top();
    }

    bool checkObj(int obj){
        vector<string> cmdtmp=cmdv;

        int ln=cmdv.size();
        for(int i=0; i<ln; i++){
            stringstream ss;
            if(cmdtmp[i]=="obj"){ss<<obj;ss>>cmdtmp[i];}
            else if(cmdtmp[i]=="allobj") return true;
        }

        vector<string> postfix;
        mid2post(cmdtmp, postfix);

        return calPostfix(postfix);

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
            else if(cmdtmp[i]=="allatom") return true;
            else{continue;}
            ss>>cmdtmp[i];
        }

        vector<string> postfix;
        mid2post(cmdtmp, postfix);

        return calPostfix(postfix);

    }

};

class DrawInfo{
public:
    vector<Present> pres;

    Color4f bgColor;

    double angleX, angleY, angleZ;
    double transX, transY, transZ;
    float MR[16];

    double orthDX;//for orth projection

    int ifDrawAxesLine, ifDrawAxes3D;
    int projType;

    double randTmp(){
        int MOD=1000;
        double rnd = rand()%MOD;
        rnd /= MOD;
        return rnd;
    }

    DrawInfo(){

        bgColor=Color4f(0.8, 0.8, 0.8, 1.0);
        pres.push_back(Present("Z>0",Color4f(randTmp(), randTmp(), randTmp(),1.0),2.0, 4));
        pres.push_back(Present("obj>=0",Color4f(randTmp(), randTmp(), randTmp(),0.5),2.0, 4));

        angleX = 0; angleY = 0; angleZ=0;
        transX = 0; transY = 0; transZ = 0;
        memset(MR, 0, sizeof(MR));
        MR[0]=1; MR[5]=1; MR[10]=1; MR[15]=1;

        orthDX=0;

        ifDrawAxes3D=1; ifDrawAxesLine=1;
        projType=1;

    }

};

#endif // DRAWINFO_H







