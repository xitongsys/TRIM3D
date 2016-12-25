#include "threadcal.h"
#include <iostream>
using namespace std;

ThreadCal::ThreadCal(){}

void ThreadCal::load(MC *pmcp, GLWT *pglp){
    stopped = false;
    pmc = pmcp; pgl = pglp;
}

void ThreadCal::run(){
    for(int i=0; i<pmc->ions.size(); i++){
        for(int j=0; j<pmc->ionNum[i]; j++){
            if(stopped) return;
            curi=i; curj=j;
            mutexLock.lock();
            pmc->runOne(i,j);
            mutexLock.unlock();
            emit signal_fresh(i,j);
        }
    }
}

void ThreadCal::stop(){
    stopped = true;
    curi=0; curj=0;
}

