#ifndef PLOTWT_H
#define PLOTWT_H

#include <QWidget>
#include "mc.h"
#include "plotinfo.h"
#include "plotwindow.h"

namespace Ui {
class PlotWT;
}

class PlotWT : public QWidget
{
    Q_OBJECT

public:
    explicit PlotWT(QWidget *parent = 0);
    ~PlotWT();    
    void paintEvent(QPaintEvent *event);

    PlotWindow *pp;

private:
    Ui::PlotWT *ui;
};

#endif // PLOTWT_H
