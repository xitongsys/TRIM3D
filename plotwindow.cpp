#include "plotwindow.h"
#include "ui_plotwindow.h"
#include "datainfo.h"
#include <sstream>
#include "global.h"
#include <QColorDialog>
#include <QFileDialog>
#include <fstream>

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

        ss.clear(); ss.str(""); ss<<cd.plotInfo.plotPresV[i].type;
        ss>>str;
        ui->plotPresTW->setItem(i, 3, new QTableWidgetItem(str.c_str()));

        double r=cd.plotInfo.plotPresV[i].col.r;
        double g=cd.plotInfo.plotPresV[i].col.g;
        double b=cd.plotInfo.plotPresV[i].col.b;
        double a=cd.plotInfo.plotPresV[i].col.a;
        QColor qcolor(r*255, g*255, b*255, a*255);
        ui->plotPresTW->item(i,1)->setBackgroundColor(qcolor);

    }

    freshSD();

}

void PlotWindow::freshSD(){
    if(cd.pmc==NULL)return;
    double xL=cd.pmc->xmax-cd.pmc->xmin, yL=cd.pmc->ymax-cd.pmc->ymin, zL=cd.pmc->zmax-cd.pmc->zmin;
    double L=sqrt(xL*xL + yL*yL + zL*zL);

    double ldis=cd.plotInfo.posL, rdis=cd.plotInfo.posR;

    ui->leftSD->setRange(0, 1000);
    ui->leftSD->setValue((ldis)/(2*L)*1000);

    ui->rightSD->setRange(0, 1000);
    ui->rightSD->setValue((rdis)/(2*L)*1000);
}

PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::on_leftSD_actionTriggered(int action)
{

    double xL=cd.pmc->xmax-cd.pmc->xmin, yL=cd.pmc->ymax-cd.pmc->ymin, zL=cd.pmc->zmax-cd.pmc->zmin;
    double L=sqrt(xL*xL + yL*yL + zL*zL);

    double ldis = ui->leftSD->value()*2*L/1000.0;
    cd.plotInfo.posL = ldis;

    emit signal_repaint();
    plotGraphe();

}

void PlotWindow::on_rightSD_actionTriggered(int action)
{

    double xL=cd.pmc->xmax-cd.pmc->xmin, yL=cd.pmc->ymax-cd.pmc->ymin, zL=cd.pmc->zmax-cd.pmc->zmin;
    double L=sqrt(xL*xL + yL*yL + zL*zL);

    double rdis = ui->rightSD->value()*2*L/1000.0;
    cd.plotInfo.posR = rdis;

    emit signal_repaint();
    plotGraphe();
}

void PlotWindow::on_addPlot_clicked()
{
    cd.plotInfo.plotPresV.push_back(Present("allatom", Color4f(1,0,0,0.5), 0, 0));
    freshPW();

}

void PlotWindow::on_sliceEd_textChanged(const QString &arg1)
{
    string str;
    stringstream ss;
    str=arg1.toStdString();
    ss<<str;
    int tmp=0;
    ss>>tmp;
    if(tmp<0) tmp=0;
    else if(tmp>=cd.MAXPLOTSLICE) tmp=cd.MAXPLOTSLICE;
    cd.plotInfo.slice=tmp;

    ss.clear(); ss.str("");
    ss<<tmp;
    ss>>str;
    ui->sliceEd->setText(str.c_str());
}

void PlotWindow::plotGraphe(){
    if(cd.pmc==NULL) return;

    double xL=cd.pmc->xmax-cd.pmc->xmin, yL=cd.pmc->ymax-cd.pmc->ymin, zL=cd.pmc->zmax-cd.pmc->zmin;
    double L=sqrt(xL*xL + yL*yL + zL*zL);
    double cx=(cd.pmc->xmax+cd.pmc->xmin)/2, cy=(cd.pmc->ymax+cd.pmc->ymin)/2, cz=(cd.pmc->zmax+cd.pmc->zmin)/2;
    Vect cp(cx,cy,cz);
    Vect dir=cd.plotInfo.plotDir;
    dir.normalize();
    Vect vo=dir*(-L); vo=vo + cp;
    Vect ve=dir*(2*L); ve=vo + ve;

    int slice=cd.plotInfo.slice;
    double bgn=min(cd.plotInfo.posL, cd.plotInfo.posR), end=max(cd.plotInfo.posL,cd.plotInfo.posR);
    double step=(end-bgn)/slice;

    memset(cd.plotData, 0, sizeof(cd.plotData));

    for(int p=0; p<cd.plotInfo.plotPresV.size(); p++){
        for(int i=0; i<cd.pmc->record.size(); i++){
            for(int j=0; j<cd.pmc->record[i].size(); j++){
                if(cd.plotInfo.plotPresV[p].check(cd.pmc->record[i][j])){
                    Vect apos=cd.pmc->record[i][j].pos;
                    Vect va = apos - vo;
                    double la = va*dir;
                    if(la<bgn || la>end) continue;
                    la = la - bgn;
                    cd.plotData[p][(int)abs(la/step)]++;
                }
            }
        }
    }

    ui->plotWT->repaint();

}

void PlotWindow::on_plotBT_clicked()
{
    plotGraphe();
}

void PlotWindow::on_plotPresTW_cellChanged(int row, int column)
{
    if(column==1) return;

    string str=ui->plotPresTW->item(row,column)->text().toStdString();
    if(column==0 && str.size()==0){
        cd.plotInfo.plotPresV.erase(cd.plotInfo.plotPresV.begin() + row);
        freshPW();
        return;
    }

    if(column==0){
        cd.plotInfo.plotPresV[row].cmd = str;
        cd.plotInfo.plotPresV[row].cmdSplit();
    }

    if(column==2){
        double alpha=1; stringstream ss; ss<<str;
        ss>>alpha;
        cd.plotInfo.plotPresV[row].col.a = alpha;
    }

    if(column==3){
        int type=0; stringstream ss; ss<<str;
        ss>>type;
        cd.plotInfo.plotPresV[row].type = type;
    }
}


void PlotWindow::on_plotPresTW_cellClicked(int row, int column)
{
    if(column==1){
        Color4f col=cd.plotInfo.plotPresV[row].col;
        int r=255*col.r, g=255*col.g, b=255*col.b;
        QColor c=QColorDialog::getColor(QColor(r,g,b));
        if(!c.isValid()) return;

        double alpha=cd.plotInfo.plotPresV[row].col.a;
        Color4f color((float)c.red()/255, (float)c.green()/255, (float)c.blue()/255, alpha);

        cd.plotInfo.plotPresV[row].col = color;
        freshPW();
    }
}

void PlotWindow::on_exportTextBT_clicked()
{
    if(cd.pmc==NULL) return;
    QFileDialog *fd=new QFileDialog(this);
    fd->setWindowTitle("Export Txt");
    fd->setNameFilter("all(*.jpg)");
    fd->setViewMode(QFileDialog::List);
    if(fd->exec()==QDialog::Accepted){
        QStringList flist=fd->selectedFiles();
        QString name=flist[0];
        ofstream of; of.open(name.toStdString());
        double step=abs(cd.plotInfo.posR - cd.plotInfo.posL)/cd.plotInfo.slice;
        for(int i=0; i<cd.plotInfo.slice; i++){
            of<<step*i<<" ";
            for(int p=0; p<cd.plotInfo.plotPresV.size();p++){
                of<<cd.plotData[p][i]<<" ";
            }
            of<<endl;
        }

        of.close();
    }
}

void PlotWindow::on_exportPicBT_clicked()
{
    if(cd.pmc==NULL) return;
    QFileDialog *fd=new QFileDialog(this);
    fd->setWindowTitle("Export Txt");
    fd->setNameFilter("all(*.*)");
    fd->setViewMode(QFileDialog::List);
    if(fd->exec()==QDialog::Accepted){
        QStringList flist=fd->selectedFiles();
        QString name=flist[0];
        name = name + ".jpg";
        QPixmap::grabWidget((ui->plotWT)).save(name);
    }
}
