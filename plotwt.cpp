#include "plotwt.h"
#include "ui_plotwt.h"
#include <QPainter>
#include "global.h"
#include "datainfo.h"

PlotWT::PlotWT(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotWT)
{
    ui->setupUi(this);
    pp=(PlotWindow*)parent;

}

PlotWT::~PlotWT()
{
    delete ui;
}

void PlotWT::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    painter.drawRect(0,0,width()-1,height()-1);

    int slice=cd.plotInfo.slice;

    int wW=width(), wH=height();
    int ymax=-1;
    for(int i=0; i<cd.plotInfo.plotPresV.size(); i++){
        ymax=(*max_element(&cd.plotData[i][0], &cd.plotData[i][0]+slice));
    }

    ymax=(double)ymax*1.2;

    int indent=50;
    int ox=indent, oy=wH-indent;
    int gW=wW - 2*indent, gH=wH - 2*indent;

    for(int p=0; p<cd.plotInfo.plotPresV.size(); p++){
        for(int xi=0; xi<slice; xi++){
            int x = ox + ((double)xi/slice)*gW;
            int y = oy - ((double)cd.plotData[p][xi]/ymax)*gH;
            painter.drawRect(x,y,2,2);
        }
    }

}
