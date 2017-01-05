#include "threadcal.h"
#include <iostream>
#include <time.h>
using namespace std;

ThreadCal::ThreadCal(){}

void ThreadCal::load(MC *pm, GLWT *pglp){
    stopped = false;
    pgl = pglp;  pmc=pm;
}

void ThreadCal::run(){
    srand(time(NULL));
    int i=0,j=0;
    for(i=0; i<(int)pmc->ions.size(); i++){
        for(j=0; j<pmc->ionNum[i]; j++){
            if(stopped) return;
            curi=i; curj=j;
            mutexLock.lock();
            pmc->runOne(i,j);
            mutexLock.unlock();
            emit signal_fresh(i,j);
        }
    }
    stopped=true;
    emit signal_fresh(i-1,j-1);

}

void ThreadCal::stop(){
    stopped = true;
    curi=0; curj=0;
}

bool ThreadCal::isStop(){
    return stopped;
}

