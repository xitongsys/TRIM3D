#include "plotwt.h"
#include "ui_plotwt.h"
#include <QPainter>

PlotWT::PlotWT(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotWT)
{
    ui->setupUi(this);
}

PlotWT::~PlotWT()
{
    delete ui;
}

void PlotWT::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawRect(0,0,width()-1,height()-1);
}
