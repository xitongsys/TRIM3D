#include "sample.h"
#include "object3d.h"
#include "global.h"

Sample::Sample(){
    xmin = INT_MAX; xmax = INT_MIN;
    ymin = INT_MAX; ymax = INT_MIN;
    zmin = INT_MAX; zmax = INT_MIN;
}

void Sample::loadFile(char* fname){
    ifstream fin(fname);
    int cur;

    while(!fin.eof()){
        char buff[1024];
        memset(buff, 0, sizeof(buff));
        fin.getline(buff, 1024);
        string str(buff);
        stringstream ss(str);
        string head;
        ss>>head;
        if(head.size()<1 || head[0]=='#') continue;
        if(head=="o"){
            cur = objs.size();
            objs.push_back(Object3D());
            ss>>objs[cur].name;
        }
        else if(head=="v"){
            double x,y,z;
            ss>>x>>y>>z;
            xmin=min(xmin, x); xmax=max(xmax, x);
            ymin=min(ymin, y); ymax=max(ymax, y);
            zmin=min(zmin, z); zmax=max(zmax, z);
            objs[cur].points.push_back(Vect(x,y,z));
        }
        else if(head=="vn"){
            double x,y,z;
            ss>>x>>y>>z;

            objs[cur].vnorms.push_back(Vect(x,y,z));
        }
        else if(head=="f"){
            int lf = objs[cur].faces.size();
            objs[cur].faces.push_back(Face());
            while(!ss.eof()){
                string tmp;
                ss>>tmp;

                int lt = tmp.size();
                int i=0,j=0;
                vector<string> vs;
                while(i<lt && j<lt){
                    while(j<lt && tmp[j]!='/')j++;
                    vs.push_back(tmp.substr(i,j-i));
                    j++; i=j;
                }


                if(vs.size()>=1){
                    stringstream s2i(vs[0]);
                    int ind; s2i>>ind;
                    objs[cur].faces[lf].vertex.push_back(ind-1);
                    if(vs.size()>=3){
                        s2i.clear(); s2i.str("");s2i<<vs[2]; s2i>>ind;
                        objs[cur].faces[lf].vnorm.push_back(ind-1);
                    }
                    else{
                        objs[cur].faces[lf].vnorm.push_back(-1);
                    }
                }
            }

            if(objs[cur].faces[lf].vertex.size()>=3){
                int vi0 = objs[cur].faces[lf].vertex[0];
                int vi1 = objs[cur].faces[lf].vertex[1];
                int vi2 = objs[cur].faces[lf].vertex[2];
                Vect v01 = objs[cur].points[vi1] - objs[cur].points[vi0];
                Vect v12 = objs[cur].points[vi2] - objs[cur].points[vi1];
                Vect vnorm = v01^v12;
                vnorm.normalize();
                int lv = objs[cur].vnorms.size();
                objs[cur].vnorms.push_back(vnorm);

                for(int i=0; i<objs[cur].faces[lf].vertex.size(); i++){
                    if(objs[cur].faces[lf].vnorm[i]<0){
                        objs[cur].faces[lf].vnorm[i]=lv;
                    }
                }
            }


        }

    }
    fin.close();
}




