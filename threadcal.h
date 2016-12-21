#ifndef THREADCAL_H
#define THREADCAL_H

#include <QThread>
#include "glwt.h"
#include "mc.h"


class ThreadCal : public QThread
{
    Q_OBJECT
public:
    MC *pmc;
    GLWT *pgl;
    volatile int curi, curj;

    ThreadCal();
    void load(MC *pmcp, GLWT *pglp);
    void stop();


signals:
    void signal_fresh(int curi, int curj);

protected:
    void run();

private:
    volatile bool stopped;
};

#endif // THREADCAL_H
