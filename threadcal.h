#ifndef THREADCAL_H
#define THREADCAL_H

#include <QThread>
#include "glwt.h"
#include "mc.h"

class GLWT;

class ThreadCal : public QThread
{
    Q_OBJECT
public:
    MC *pmc;
    GLWT *pgl;
    volatile int curi, curj;

    ThreadCal();
    void load(MC *pm, GLWT *pglp);
    void stop();
    bool isStop();


signals:
    void signal_fresh(int curi, int curj);

protected:
    void run();

private:
    volatile bool stopped;
};

#endif // THREADCAL_H
