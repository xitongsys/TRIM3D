#include "colordialog.h"
#include "ui_colordialog.h"
#include <QColorDialog>
#include "mainwindow.h"
#include <iostream>
#include <sstream>
using namespace std;

ColorDialog::ColorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorDialog)
{
    pp = (MainWindow*)parent;
    ui->setupUi(this);


    int r,g,b;
    r = pp->pDrawInfo->bgColor.r*255;
    g = pp->pDrawInfo->bgColor.g*255;
    b = pp->pDrawInfo->bgColor.b*255;
    char buf[1024];
    sprintf(buf, "QPushButton{background-color:rgb(%d,%d,%d);}", r,g,b);
    ui->bgBT->setStyleSheet(buf);
    ui->bgASB->setValue(pp->pDrawInfo->bgColor.a*100);

    r = pp->pDrawInfo->objColor.r*255;
    g = pp->pDrawInfo->objColor.g*255;
    b = pp->pDrawInfo->objColor.b*255;
    sprintf(buf, "QPushButton{background-color:rgb(%d,%d,%d);}", r,g,b);
    ui->structureBT->setStyleSheet(buf);
    ui->objASB->setValue(pp->pDrawInfo->objColor.a*100);

    freshPres();

}

void ColorDialog::freshPres(){
    int lp=pp->pDrawInfo->pres.size();
    ui->presWT->setRowCount(lp);
    for(int i=0; i<lp; i++){
        stringstream ss;
        string str;

        str=pp->pDrawInfo->pres[i].cmd;
        QString stmp; stmp=stmp.fromLocal8Bit(str.c_str());
        ui->presWT->setItem(i, 0, new QTableWidgetItem(stmp));

        string r,g,b,a;
        ss.clear(); ss.str(""); ss<<pp->pDrawInfo->pres[i].col.r; ss>>r;
        ss.clear(); ss.str(""); ss<<pp->pDrawInfo->pres[i].col.g; ss>>g;
        ss.clear(); ss.str(""); ss<<pp->pDrawInfo->pres[i].col.b; ss>>b;
        ss.clear(); ss.str(""); ss<<pp->pDrawInfo->pres[i].col.a; ss>>a;
        str=r + " " + g + " " + b + " " + a;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->presWT->setItem(i, 1, new QTableWidgetItem(stmp));


        ss.clear(); ss.str("");
        ss<<pp->pDrawInfo->pres[i].R;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->presWT->setItem(i, 2, new QTableWidgetItem(stmp));


        ss.clear(); ss.str("");
        ss<<pp->pDrawInfo->pres[i].slice;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->presWT->setItem(i, 3, new QTableWidgetItem(stmp));

    }
}

ColorDialog::~ColorDialog()
{
    delete ui;
}

void ColorDialog::on_bgBT_clicked(){
    QColor c = QColorDialog::getColor();
    char buf[1024];
    sprintf(buf, "QPushButton{background-color:rgb(%d,%d,%d);}", c.red(), c.green(), c.blue());
    ui->bgBT->setStyleSheet(buf);
    pp->pDrawInfo->bgColor.r = (float)c.red()/255;
    pp->pDrawInfo->bgColor.g = (float)c.green()/255;
    pp->pDrawInfo->bgColor.b = (float)c.blue()/255;
}

void ColorDialog::on_ColorDialog_accepted()
{
   pp->freshGL();
}


void ColorDialog::on_structureBT_clicked()
{
    QColor c = QColorDialog::getColor();
    char buf[1024];
    sprintf(buf, "QPushButton{background-color:rgb(%d,%d,%d);}", c.red(), c.green(), c.blue());
    ui->structureBT->setStyleSheet(buf);
    pp->pDrawInfo->objColor.r = (float)c.red()/255;
    pp->pDrawInfo->objColor.g = (float)c.green()/255;
    pp->pDrawInfo->objColor.b = (float)c.blue()/255;
}

void ColorDialog::on_applyBT_clicked(){
    pp->freshGL();
}

void ColorDialog::on_bgASB_editingFinished(){
    pp->pDrawInfo->bgColor.a = float(ui->bgASB->value())/100;
}

void ColorDialog::on_objASB_editingFinished()
{
    pp->pDrawInfo->objColor.a = float(ui->objASB->value())/100;
}

void ColorDialog::on_addPresBT_clicked()
{
    pp->pDrawInfo->pres.push_back(Present("all", Color4f(1,1,1,1),2.0, 5));
    freshPres();

}

void ColorDialog::on_presWT_cellChanged(int row, int column)
{
    if(column==1) return;
    string str=ui->presWT->item(row,column)->text().toStdString();

    if(column==0 && str.size()==0){
        pp->pDrawInfo->pres.erase(pp->pDrawInfo->pres.begin() + row);
        freshPres();
        return;
    }

    if(column==0){
        pp->pDrawInfo->pres[row].cmd = str;
        pp->pDrawInfo->pres[row].cmdSplit();
    }

    if(column==2){
        double R; stringstream ss; ss<<str;
        ss>>R;
        pp->pDrawInfo->pres[row].R = R;
    }

    if(column==3){
        int s; stringstream ss; ss<<str;
        ss>>s;
        pp->pDrawInfo->pres[row].slice = s;
    }

}


void ColorDialog::on_presWT_cellClicked(int row, int column)
{
    if(column==1){
        QColor c=QColorDialog::getColor();
        Color4f color((float)c.red()/255, (float)c.green()/255, (float)c.blue()/255, (float)c.alpha()/255);
        pp->pDrawInfo->pres[row].col = color;
        freshPres();
    }
}
