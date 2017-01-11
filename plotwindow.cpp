#include "plotwindow.h"
#include "ui_plotwindow.h"
#include "datainfo.h"
#include <sstream>

PlotWindow::PlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);
    pp=(MainWindow*)parent;
    freshPW();

}

void PlotWindow::freshPW(){
    stringstream ss;
    string str;
    ss<<cd.plotInfo.slice; ss>>str;
    ui->sliceEd->setText(str.c_str());
}

PlotWindow::~PlotWindow()
{
    delete ui;
}
