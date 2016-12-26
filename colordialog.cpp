#include "colordialog.h"
#include "ui_colordialog.h"
#include <QColorDialog>
#include "mainwindow.h"
#include <iostream>
using namespace std;

ColorDialog::ColorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorDialog)
{
    pp = (MainWindow*)parent;
    ui->setupUi(this);
    for(int i=1; i<=99; i++){
        stringstream ss;
        ss<<i; string str; ss>>str;
        ui->atomZCB->addItem(str.c_str());
    }
    this->on_atomZCB_activated("1");
    int r,g,b;
    r = pp->pDrawInfo->bgColor.r*255;
    g = pp->pDrawInfo->bgColor.g*255;
    b = pp->pDrawInfo->bgColor.b*255;
    char buf[1024];
    sprintf(buf, "QPushButton{background-color:rgb(%d,%d,%d);}", r,g,b);
    ui->bgBT->setStyleSheet(buf);
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

void ColorDialog::on_atomZCB_activated(const QString &arg1){
    int r,g,b;
    stringstream ss; ss<<arg1.toStdString();
    int Z; ss>>Z;
    r = pp->pDrawInfo->AtomColorTable[Z].r*255;
    g = pp->pDrawInfo->AtomColorTable[Z].g*255;
    b = pp->pDrawInfo->AtomColorTable[Z].b*255;
    char buf[1024];
    sprintf(buf, "QPushButton{background-color:rgb(%d,%d,%d);}", r, g, b);
    ui->atomColorBT->setStyleSheet(buf);
}

void ColorDialog::on_atomColorBT_clicked(){
    int Z;
    string Zs=ui->atomZCB->currentText().toStdString();
    stringstream ss; ss<<Zs; ss>>Z;
    QColor c = QColorDialog::getColor();
    char buf[1024];
    sprintf(buf, "QPushButton{background-color:rgb(%d,%d,%d);}", c.red(), c.green(), c.blue());
    ui->atomColorBT->setStyleSheet(buf);
    pp->pDrawInfo->AtomColorTable[Z].r = (float)c.red()/255;
    pp->pDrawInfo->AtomColorTable[Z].g = (float)c.green()/255;
    pp->pDrawInfo->AtomColorTable[Z].b = (float)c.blue()/255;

}
