#include "colordialog.h"
#include "ui_colordialog.h"
#include <QColorDialog>
#include "mainwindow.h"
#include <iostream>
#include <sstream>
#include "datainfo.h"
using namespace std;

ColorDialog::ColorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorDialog)
{
    pp = (MainWindow*)parent;
    ui->setupUi(this);


    int r,g,b;
    r = cd.drawInfo.bgColor.r*255;
    g = cd.drawInfo.bgColor.g*255;
    b = cd.drawInfo.bgColor.b*255;
    char buf[1024];
    sprintf(buf, "QPushButton{background-color:rgb(%d,%d,%d);}", r,g,b);
    ui->bgBT->setStyleSheet(buf);

    freshPres();

}

void ColorDialog::freshPres(){
    int lp=cd.drawInfo.pres.size();
    ui->presWT->setRowCount(lp);
    for(int i=0; i<lp; i++){
        stringstream ss;
        string str;

        str=cd.drawInfo.pres[i].cmd;
        QString stmp; stmp=stmp.fromLocal8Bit(str.c_str());
        ui->presWT->setItem(i, 0, new QTableWidgetItem(stmp));

        string r,g,b,a;
        ss.clear(); ss.str(""); ss<<cd.drawInfo.pres[i].col.r; ss>>r;
        ss.clear(); ss.str(""); ss<<cd.drawInfo.pres[i].col.g; ss>>g;
        ss.clear(); ss.str(""); ss<<cd.drawInfo.pres[i].col.b; ss>>b;
        ss.clear(); ss.str(""); ss<<cd.drawInfo.pres[i].col.a; ss>>a;
        str=r + " " + g + " " + b;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->presWT->setItem(i, 1, new QTableWidgetItem(stmp));
        stmp=stmp.fromLocal8Bit(a.c_str());
        ui->presWT->setItem(i, 4, new QTableWidgetItem(stmp));


        ss.clear(); ss.str("");
        ss<<cd.drawInfo.pres[i].R;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->presWT->setItem(i, 2, new QTableWidgetItem(stmp));


        ss.clear(); ss.str("");
        ss<<cd.drawInfo.pres[i].slice;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->presWT->setItem(i, 3, new QTableWidgetItem(stmp));

        double dr,dg,db,da;
        dr = cd.drawInfo.pres[i].col.r;
        dg = cd.drawInfo.pres[i].col.g;
        db = cd.drawInfo.pres[i].col.b;
        da = cd.drawInfo.pres[i].col.a;
        QColor qcolor(dr*255,dg*255,db*255,da*255);
        ui->presWT->item(i, 1)->setBackgroundColor(qcolor);

    }
}

ColorDialog::~ColorDialog()
{
    delete ui;
}

void ColorDialog::on_bgBT_clicked(){
    QColor c=QColorDialog::getColor(QColor(cd.drawInfo.bgColor.r*255, cd.drawInfo.bgColor.g*255, cd.drawInfo.bgColor.b*255));
    if(!c.isValid()) return;
    char buf[1024];
    sprintf(buf, "QPushButton{background-color:rgb(%d,%d,%d);}", c.red(), c.green(), c.blue());
    ui->bgBT->setStyleSheet(buf);
    cd.drawInfo.bgColor.r = (float)c.red()/255;
    cd.drawInfo.bgColor.g = (float)c.green()/255;
    cd.drawInfo.bgColor.b = (float)c.blue()/255;
}

void ColorDialog::on_ColorDialog_accepted()
{
   pp->freshGL();
}


void ColorDialog::on_applyBT_clicked(){
    pp->freshGL();
}


void ColorDialog::on_addPresBT_clicked()
{
    cd.drawInfo.pres.push_back(Present("allatom", Color4f(1,1,1,1),2.0, 5));
    freshPres();

}

void ColorDialog::on_presWT_cellChanged(int row, int column)
{
    if(column==1) return;
    string str=ui->presWT->item(row,column)->text().toStdString();

    if(column==0 && str.size()==0){
        cd.drawInfo.pres.erase(cd.drawInfo.pres.begin() + row);
        freshPres();
        return;
    }

    if(column==0){
        cd.drawInfo.pres[row].cmd = str;
        cd.drawInfo.pres[row].cmdSplit();
    }

    if(column==2){
        double R; stringstream ss; ss<<str;
        ss>>R;
        cd.drawInfo.pres[row].R = R;
    }

    if(column==3){
        int s; stringstream ss; ss<<str;
        ss>>s;
        cd.drawInfo.pres[row].slice = s;
    }

    if(column==4){
        double alpha; stringstream ss; ss<<str;
        ss>>alpha;
        cd.drawInfo.pres[row].col.a = alpha;
    }

}


void ColorDialog::on_presWT_cellClicked(int row, int column)
{
    if(column==1){
        Color4f col=cd.drawInfo.pres[row].col;
        int r=255*col.r, g=255*col.g, b=255*col.b;
        QColor c=QColorDialog::getColor(QColor(r,g,b));
        if(!c.isValid()) return;

        double alpha=cd.drawInfo.pres[row].col.a;
        Color4f color((float)c.red()/255, (float)c.green()/255, (float)c.blue()/255, alpha);
        cd.drawInfo.pres[row].col = color;
        freshPres();
    }
}
