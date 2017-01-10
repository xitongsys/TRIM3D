#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H


#include <QMainWindow>
#include "plotinfo.h"
#include "mc.h"


namespace Ui {
class PlotWindow;
}

class PlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = 0, MC *pmcc=NULL, PlotInfo *pInfoo=NULL);
    ~PlotWindow();

    MC *pmc;
    PlotInfo *pInfo;

private:
    Ui::PlotWindow *ui;
};

#endif // PLOTWINDOW_H
