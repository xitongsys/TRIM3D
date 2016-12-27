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
    ui->atomASB->setValue(pp->pDrawInfo->AtomColorTable[1].a*100);


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

void ColorDialog::on_atomASB_editingFinished()
{
   stringstream ss(ui->atomZCB->currentText().toStdString());
   int Z; ss>>Z;
   pp->pDrawInfo->AtomColorTable[Z].a = float(ui->atomASB->value())/100;
}
