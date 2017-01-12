#include "plotwindow.h"
#include "ui_plotwindow.h"
#include "datainfo.h"
#include <sstream>
#include "global.h"
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
    int ln=cd.plotInfo.plotPresV.size();
    ui->plotPresTW->setRowCount(ln);
    for(int i=0; i<ln; i++){
        ui->plotPresTW->setItem(i, 0, new QTableWidgetItem(cd.plotInfo.plotPresV[i].cmd.c_str()));
        string tmp;
        str="";
        ss.clear(); ss.str(""); ss<<cd.plotInfo.plotPresV[i].col.r;
        ss>>tmp; str += tmp + " ";
        ss.clear(); ss.str(""); ss<<cd.plotInfo.plotPresV[i].col.g;
        ss>>tmp; str += tmp + " ";
        ss.clear(); ss.str(""); ss<<cd.plotInfo.plotPresV[i].col.b;
        ss>>tmp; str += tmp + " ";
        ui->plotPresTW->setItem(i, 1, new QTableWidgetItem(str.c_str()));

        ss.clear(); ss.str(""); ss<<cd.plotInfo.plotPresV[i].col.a;
        ss>>str;
        ui->plotPresTW->setItem(i, 2, new QTableWidgetItem(str.c_str()));

        str=cd.plotInfo.plotPresV[i].typeName[cd.plotInfo.plotPresV[i].type];
        ui->plotPresTW->setItem(i, 3, new QTableWidgetItem(str.c_str()));

    }

    freshSD();

}

void PlotWindow::freshSD(){
    if(cd.pmc==NULL)return;
    double xL=cd.pmc->xmax-cd.pmc->xmin, yL=cd.pmc->ymax-cd.pmc->ymin, zL=cd.pmc->zmax-cd.pmc->zmin;
    double L=sqrt(xL*xL + yL*yL + zL*zL);

    double ldis=cd.plotInfo.posL, rdis=cd.plotInfo.posR;

    ui->leftSD->setRange(0, 100);
    ui->leftSD->setValue((ldis)/(2*L)*100);

    ui->rightSD->setRange(0, 100);
    ui->rightSD->setValue((rdis)/(2*L)*100);
}

PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::on_leftSD_actionTriggered(int action)
{

    double xL=cd.pmc->xmax-cd.pmc->xmin, yL=cd.pmc->ymax-cd.pmc->ymin, zL=cd.pmc->zmax-cd.pmc->zmin;
    double L=sqrt(xL*xL + yL*yL + zL*zL);

    double ldis = ui->leftSD->value()*2*L/100.0;
    cd.plotInfo.posL = ldis;

    emit signal_repaint();

}

void PlotWindow::on_rightSD_actionTriggered(int action)
{

    double xL=cd.pmc->xmax-cd.pmc->xmin, yL=cd.pmc->ymax-cd.pmc->ymin, zL=cd.pmc->zmax-cd.pmc->zmin;
    double L=sqrt(xL*xL + yL*yL + zL*zL);

    double rdis = ui->rightSD->value()*2*L/100.0;
    cd.plotInfo.posR = rdis;

    emit signal_repaint();
}
