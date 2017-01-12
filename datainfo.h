#ifndef DATAINFO_H
#define DATAINFO_H


#include "mc.h"
#include "global.h"
#include "drawinfo.h"
#include "qtdata.h"
#include "threadcal.h"
#include "plotinfo.h"
#include <vector>

class DataInfo
{

public:
    MC *pmc;
    ThreadCal *tc;

    DrawInfo drawInfo;
    QTInputData qtdata;
    PlotInfo plotInfo;

    const static int MAXPLOTPRES=100, MAXPLOTSLICE=10000;
    int plotData[MAXPLOTPRES][MAXPLOTSLICE];


public:
    DataInfo();
};

#endif // DATAINFO_H
