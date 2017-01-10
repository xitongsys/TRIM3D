#ifndef PLOTWT_H
#define PLOTWT_H

#include <QWidget>
#include "mc.h"
#include "plotinfo.h"

namespace Ui {
class PlotWT;
}

class PlotWT : public QWidget
{
    Q_OBJECT

public:
    explicit PlotWT(QWidget *parent = 0, MC *pmcc=NULL, PlotInfo *pInfoo=NULL);
    ~PlotWT();    
    void paintEvent(QPaintEvent *event);

    MC *pmc;
    PlotInfo *pInfo;

private:
    Ui::PlotWT *ui;
};

#endif // PLOTWT_H
