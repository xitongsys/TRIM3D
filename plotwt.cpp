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
    int ymax=0;
    for(int i=0; i<cd.plotInfo.plotPresV.size(); i++){
        ymax=max(ymax, (*max_element(&cd.plotData[i][0], &cd.plotData[i][0]+slice)));
    }

    ymax=(double)ymax*1.2;


    int indent=50;
    int ox=indent, oy=wH-indent;
    int gW=wW - 2*indent, gH=wH - 2*indent;

    painter.setPen(QColor(0,0,0));
    painter.setBrush(QColor(255,255,255));
    painter.drawRect(indent, indent, gW, gH);
    painter.drawText(indent,indent-5, "ATOM DISTRIBUTION");

    if(ymax<=0) return;

    painter.drawText(ox,oy+indent/2,"0(A)");
    double l=abs(cd.plotInfo.posR - cd.plotInfo.posL);
    stringstream ss; ss<<(long)l;
    string str; ss>>str; str=str + "(A)";
    painter.drawText(ox+gW,oy+indent/2,str.c_str());

    painter.setPen(QColor(125,125,125));
    for(int i=1; i<10; i++){
        int x1,y1,x2,y2;
        x1=indent; x2=indent + gW;
        y1=oy - i*(gH/10); y2=y1;
        painter.drawLine(x1,y1,x2,y2);
        int num=i*(ymax/10);
        stringstream ss; ss<<num;
        string str; ss>>str;
        painter.drawText(x2,y2+5,str.c_str());
    }


    for(int p=0; p<cd.plotInfo.plotPresV.size(); p++){
        int r=cd.plotInfo.plotPresV[p].col.r*255;
        int g=cd.plotInfo.plotPresV[p].col.g*255;
        int b=cd.plotInfo.plotPresV[p].col.b*255;
        int a=cd.plotInfo.plotPresV[p].col.a*255;

        QColor qcolor(r,g,b,a);
        painter.setPen(qcolor);
        painter.setBrush(qcolor);
        for(int xi=0; xi<slice; xi++){
            int x = ox + ((double)xi/slice)*gW;
            int y = oy - ((double)cd.plotData[p][xi]/ymax)*gH;


            if(cd.plotInfo.plotPresV[p].type==0){
                painter.drawRect(x,y,gW/slice, oy-y);
            }
        }
    }

}


















