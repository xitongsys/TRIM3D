#include "plotwindow.h"
#include "ui_plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent, MC *pmcc, PlotInfo *pInfoo) :
    QMainWindow(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);
    pmc = pmcc;
    pInfo = pInfoo;
}

PlotWindow::~PlotWindow()
{
    delete ui;
}
