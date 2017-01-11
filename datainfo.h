#ifndef DATAINFO_H
#define DATAINFO_H


#include "mc.h"
#include "global.h"
#include "drawinfo.h"
#include "qtdata.h"
#include "threadcal.h"

class DataInfo
{

public:
    MC *pmc;
    ThreadCal *tc;

    DrawInfo drawInfo;
    QTInputData qtdata;


public:
    DataInfo();
};

#endif // DATAINFO_H
