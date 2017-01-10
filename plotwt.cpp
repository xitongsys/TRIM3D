#include "plotwt.h"
#include "ui_plotwt.h"
#include <QPainter>

PlotWT::PlotWT(QWidget *parent, MC *pmcc, PlotInfo *pInfoo) :
    QWidget(parent),
    ui(new Ui::PlotWT)
{
    ui->setupUi(this);
    pmc = pmcc;
    pInfo = pInfoo;
}

PlotWT::~PlotWT()
{
    delete ui;
}

void PlotWT::paintEvent(QPaintEvent *event){
    if(pmc==NULL) return;

    QPainter painter(this);
    painter.drawRect(0,0,width()-1,height()-1);
}
