#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>

#include "atomtabledialog.h"
#include "colordialog.h"
#include "helpwindow.h"
#include "plotwindow.h"
#include "datainfo.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addObjBT, SIGNAL(clicked(bool)), this, SLOT(onAddObjBt()));
    connect(ui->addEleBT, SIGNAL(clicked(bool)), this, SLOT(onAddEleBt()));
    connect(ui->addIonBT, SIGNAL(clicked(bool)), this, SLOT(onAddIonBt()));
    connect(ui->objTW, SIGNAL(cellChanged(int,int)), this, SLOT(onObjChangeTW(int,int)));
    connect(ui->objTW, SIGNAL(cellClicked(int,int)), this, SLOT(onObjCellClick(int,int)));
    connect(ui->eleTW, SIGNAL(cellChanged(int,int)), this, SLOT(onEleChangeTW(int,int)));
    connect(ui->ionTW, SIGNAL(cellChanged(int,int)), this, SLOT(onIonChangeTW(int,int)));

    ui->toolBar->addAction(ui->actionNew);
    ui->toolBar->addAction(ui->actionOpen);
    ui->toolBar->addAction(ui->actionSave);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionLoad);
    ui->toolBar->addAction(ui->actionRun);
    ui->toolBar->addAction(ui->actionStop);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionColor);
    ui->toolBar->addAction(ui->actionPlot);
    ui->toolBar->addAction(ui->actionExport_data);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionContent);
    ui->toolBar->addAction(ui->actionAbout);

    plotW=new PlotWindow(this);

    connect(cd.tc, SIGNAL(signal_fresh(int,int)), this, SLOT(slot_fresh(int,int)));
    connect(plotW, SIGNAL(signal_repaint()), this, SLOT(slot_repaint()));


}

void MainWindow::slot_fresh(int curi, int curj){
    if(ui->ifShowCB->isChecked()){
        ui->openGLWidget->repaint();
    }
    int ionNum=0, recoilNum=0;
    ionNum = cd.pmc->incidentNum;
    recoilNum = cd.pmc->recoilNum;
    stringstream ss; ss<<ionNum;
    string str; ss>>str;
    QString stmp; stmp = stmp.fromLocal8Bit(str.c_str());
    ui->ionNumEd->setText(stmp);
    ss.clear(); ss.str("");
    ss<<(recoilNum); ss>>str;
    stmp=stmp.fromLocal8Bit(str.c_str());
    ui->reNumEd->setText(stmp);

    int sum=0, sumcur=0;
    for(int i=0; i<(int)cd.pmc->ions.size(); i++){
        sum += cd.pmc->ionNum[i];
        if(i<curi){
            sumcur += cd.pmc->ionNum[i];
        }
    }
    sumcur += curj + 1;
    ui->progressBar->setValue((int)((float)sumcur/sum * 100));
    if(cd.tc->isStop()){
        ui->actionRun->setEnabled(true);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onObjCellClick(int row, int col){
    freshEleTW();
}

void MainWindow::onIonChangeTW(int row, int col){
    string str = ui->ionTW->item(row,col)->text().toLocal8Bit().toStdString();
    stringstream ss; ss<<str;
    if(row==0){
        if(str.size()==0){
            cd.qtdata.ions.erase(row + cd.qtdata.ions.begin());
            freshIonTW();
            return;
        }
        else{
            cd.qtdata.ions[col].name = str;
        }
    }
    else if(row==1){ ss>>cd.qtdata.ions[col].Z; }
    else if(row==2){ ss>>cd.qtdata.ions[col].M; }
    else if(row==3){ ss>>cd.qtdata.ions[col].number; }
    else if(row==4){ ss>>cd.qtdata.ions[col].x; }
    else if(row==5){ ss>>cd.qtdata.ions[col].y; }
    else if(row==6){ ss>>cd.qtdata.ions[col].z; }
    else if(row==7){ ss>>cd.qtdata.ions[col].vx; }
    else if(row==8){ ss>>cd.qtdata.ions[col].vy; }
    else if(row==9){ ss>>cd.qtdata.ions[col].vz; }
    else if(row==10){ ss>>cd.qtdata.ions[col].energy; }
}

void MainWindow::onObjChangeTW(int row, int col){
    if(ui->objTW->item(row,col)->text().size()==0){
        cd.qtdata.objs.erase(cd.qtdata.objs.begin() + row);
        freshObjTW();
    }
    else{
        string str=ui->objTW->item(row,col)->text().toLocal8Bit().toStdString();
        if(col==0){
            cd.qtdata.objs[row].objFile=str;
        }else if(col==1){
            stringstream ss; ss<<str;
            ss>>cd.qtdata.objs[row].objDensity;
        }else if(col==2){
            stringstream ss; ss<<str;
            ss>>cd.qtdata.objs[row].scale;
        }
    }
    this->on_actionLoad_triggered();
}

void MainWindow::onEleChangeTW(int row, int col){
    int oi = ui->objTW->currentRow();
    if(oi>=0 && oi<(int)cd.qtdata.objs.size()){
        string str = ui->eleTW->item(row,col)->text().toLocal8Bit().toStdString();
        if(col==0){
            if(str.size()==0){
                cd.qtdata.objs[oi].delEle(row);
                freshEleTW();
                return;
            }
            else{
                cd.qtdata.objs[oi].elements[row].name = str;
            }
        }
        else if(col==1){
            stringstream ss; ss<<str;
            ss>>cd.qtdata.objs[oi].elements[row].Z;
        }
        else if(col==2){
            stringstream ss; ss<<str;
            ss>>cd.qtdata.objs[oi].elements[row].M;
        }
        else if(col==3){
            stringstream ss; ss<<str;
            ss>>cd.qtdata.objs[oi].elements[row].fraction;
        }
        else if(col==4){
            stringstream ss; ss<<str;
            ss>>cd.qtdata.objs[oi].elements[row].disE;
        }
    }
}

void MainWindow::freshObjTW(){
    int lo = cd.qtdata.objs.size();
    ui->objTW->setRowCount(lo);
    for(int i=0; i<lo; i++){
        QString stmp; stmp = stmp.fromLocal8Bit(cd.qtdata.objs[i].objFile.c_str());
        ui->objTW->setItem(i, 0, new QTableWidgetItem(stmp));

        cout<<cd.qtdata.objs[i].objFile<<endl;

        stringstream ss;
        string str;
        ss.clear(); ss.str(""); ss<<cd.qtdata.objs[i].objDensity;
        ss>>str;
        stmp = stmp.fromLocal8Bit(str.c_str());
        ui->objTW->setItem(i, 1, new QTableWidgetItem(stmp));

        ss.clear(); ss.str(""); ss<<cd.qtdata.objs[i].scale;
        ss>>str;
        stmp = stmp.fromLocal8Bit(str.c_str());
        ui->objTW->setItem(i, 2, new QTableWidgetItem(stmp));

    }
}

void MainWindow::freshEleTW(){
    int curR = ui->objTW->currentRow();
    int lo = cd.qtdata.objs.size();
    if(curR<0 || curR>=lo){
        ui->eleTW->setRowCount(0);
        return;
    }

    int le = cd.qtdata.objs[curR].elements.size();
    ui->eleTW->setRowCount(le);
    for(int i=0; i<le; i++){
        stringstream ss;
        string str;
        str=cd.qtdata.objs[curR].elements[i].name;
        QString stmp; stmp = stmp.fromLocal8Bit(str.c_str());
        ui->eleTW->setItem(i, 0, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.objs[curR].elements[i].Z;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->eleTW->setItem(i, 1, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.objs[curR].elements[i].M;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->eleTW->setItem(i, 2, new QTableWidgetItem(stmp));

        ss.clear(); ss.str(""); ss<<cd.qtdata.objs[curR].elements[i].fraction;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->eleTW->setItem(i, 3, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.objs[curR].elements[i].disE;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->eleTW->setItem(i, 4, new QTableWidgetItem(stmp));
    }
}

void MainWindow::freshIonTW(){
    int li = cd.qtdata.ions.size();
    ui->ionTW->setColumnCount(li);
    for(int i=0; i<li; i++){
        stringstream ss;
        string str; QString stmp;
        ss<<cd.qtdata.ions[i].name; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(0,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.ions[i].Z; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(1,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.ions[i].M; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(2,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.ions[i].number; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(3,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.ions[i].x; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(4,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.ions[i].y; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(5,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.ions[i].z; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(6,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.ions[i].vx; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(7,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.ions[i].vy; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(8,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.ions[i].vz; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(9,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<cd.qtdata.ions[i].energy; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(10,i, new QTableWidgetItem(stmp));
    }
}


void MainWindow::onAddObjBt(){
    QFileDialog *fd=new QFileDialog(this);
    fd->setNameFilter("obj file(*.obj)");
    fd->setViewMode(QFileDialog::List);
    QStringList flist;
    ui->objTW->setColumnWidth(0,110);
    if(fd->exec()==QDialog::Accepted){
        flist=fd->selectedFiles();
        for(int i=0; i<flist.size(); i++){
            QString fname = flist[i];
            cd.qtdata.objs.push_back(QTObj(fname.toLocal8Bit().toStdString(), 1, 1));
        }

        freshObjTW();
    }
    else{
        fd->close();
    }

}

void MainWindow::onAddEleBt(){
    int oi = ui->objTW->currentRow();
    AtomTableDialog atomDialog(this);
    if(atomDialog.exec()==QDialog::Accepted){
        if(oi>=0 && oi<(int)cd.qtdata.objs.size()){
            cd.qtdata.objs[oi].addEle(QTEle(atomDialog.name, atomDialog.Z, atomDialog.mass, 1.0, atomDialog.disp));
            freshEleTW();
        }
    }
}

void MainWindow::onAddIonBt(){

    AtomTableDialog atomDialog(this);
    if(atomDialog.exec()==QDialog::Accepted){
        cd.qtdata.ions.push_back(QTIon(atomDialog.name, atomDialog.Z, atomDialog.mass, 1, 0,0,0, 0,0,1, 1000));
        freshIonTW();
    }
}

void MainWindow::on_actionNew_triggered(){
    cd.qtdata.clear();
    freshEleTW();
    freshIonTW();
    freshObjTW();
    this->setWindowTitle("TRIM3D");
}

void MainWindow::on_actionOpen_triggered(){
    QFileDialog *fd=new QFileDialog(this);
    fd->setWindowTitle("Open input file");
    fd->setNameFilter("trim3d file(*.t3d)");
    fd->setViewMode(QFileDialog::List);
    QStringList flist;
    if(fd->exec()==QDialog::Accepted){
        flist=fd->selectedFiles();
        QString fname = flist[0];
        this->on_actionNew_triggered();
        cd.qtdata.loadInput(fname.toLocal8Bit().toStdString());
        freshIonTW();
        freshObjTW();
        this->setWindowTitle(fname);
        this->on_actionLoad_triggered();
    }
    else{
        fd->close();
    }

    //this->on_actionLoad_triggered();

}

void MainWindow::on_actionExit_triggered()
{
   exit(0);
}

void MainWindow::on_actionSave_triggered(){
    string path = cd.qtdata.filePath;
    if(path.size()!=0){
        cd.qtdata.saveInput();
    }
    else{
        QFileDialog *fd=new QFileDialog(this);
        fd->setWindowTitle("Save input file");
        fd->setNameFilter("trim3d file(*.t3d)");
        fd->setViewMode(QFileDialog::List);
        if(fd->exec()==QDialog::Accepted){
            QStringList flist = fd->selectedFiles();
            QString name = flist[0];
            cd.qtdata.filePath = name.toLocal8Bit().toStdString();
            cd.qtdata.saveInput();
        }
    }

    QString stmp; stmp=stmp.fromLocal8Bit(cd.qtdata.filePath.c_str());
    this->setWindowTitle(stmp);

}

void MainWindow::on_actionLoad_triggered(){
    this->on_actionSave_triggered();
    if(cd.pmc!=NULL){
        delete cd.pmc;
    }
    if(cd.qtdata.filePath.size()==0) return;


    cd.pmc = new MC(cd.qtdata.filePath, "SCOEF.88");
    //ui->openGLWidget->resetView();

    if(ui->actionQuick->isChecked()){
        cd.pmc->runMode=1;
    }

    ui->openGLWidget->repaint();
}

void MainWindow::on_actionRun_triggered(){
    if(cd.pmc==NULL) return;
    cd.tc->stop();
    this->on_actionLoad_triggered();
    if(cd.pmc->objs.size()<=0 || cd.pmc->ions.size()<=0) return;
    for(int i=0; i<(int)cd.pmc->objs.size();i++){
        if(cd.pmc->objs[i].elements.size()<=0)return;
    }
    cd.tc->load(cd.pmc, ui->openGLWidget);
    cd.tc->start();
    ui->actionRun->setDisabled(true);
}

void MainWindow::on_actionStop_triggered(){
    cd.tc->stop();
    ui->actionRun->setEnabled(true);
}

void MainWindow::on_actionExport_data_triggered() {
    if(cd.pmc==NULL) return;
    QFileDialog *fd=new QFileDialog(this);
    fd->setWindowTitle("Export data file");
    fd->setNameFilter("all(*.*)");
    fd->setViewMode(QFileDialog::List);
    if(fd->exec()==QDialog::Accepted){
        QStringList flist=fd->selectedFiles();
        QString name=flist[0];
        cd.qtdata.saveExport(name.toLocal8Bit().toStdString(), cd.pmc);
    }
}

void MainWindow::on_actionAbout_triggered(){
    QMessageBox message(this);
    message.setWindowTitle("About TRIM3D");
    message.setText("Trim3D v3.2\n\n developed by Zhang Xitong\nEmail: xitongsys@gmail.com\n");
    message.setIconPixmap(QPixmap(":/images/mainIcon/ico128x128.png"));
    message.exec();

}


void MainWindow::on_ifShowCB_clicked() {
    if(ui->ifShowCB->checkState()){
        cd.drawInfo.ifShow=1;
    }
    else{
        cd.drawInfo.ifShow=0;
    }
    ui->openGLWidget->repaint();
}

void MainWindow::on_actionColor_triggered(){
    ColorDialog cdlg(this);
    cdlg.exec();
}

void MainWindow::freshGL(){
    ui->openGLWidget->repaint();
}

void MainWindow::on_actionContent_triggered()
{
    hw.show();
}

void MainWindow::on_actionAxesLine_triggered()
{
    cd.drawInfo.ifDrawAxesLine *=-1;
    ui->openGLWidget->repaint();
}

void MainWindow::on_actionAxes_3D_triggered()
{
    cd.drawInfo.ifDrawAxes3D *= -1;
    ui->openGLWidget->repaint();
}

void MainWindow::on_actionPerspective_triggered()
{
    cd.drawInfo.projType*=-1;
    ui->actionOrthographic->setChecked(false);
    ui->actionPerspective->setDisabled(true);
    ui->actionOrthographic->setEnabled(true);


    ui->openGLWidget->repaint();
}


void MainWindow::on_actionOrthographic_triggered()
{
    cd.drawInfo.projType*=-1;
    ui->actionPerspective->setChecked(false);
    ui->actionPerspective->setEnabled(true);
    ui->actionOrthographic->setDisabled(true);

    ui->openGLWidget->repaint();
}

void MainWindow::on_zoomOut_clicked()
{
    if(cd.drawInfo.projType==1){
        double dz=(cd.pmc->zmax - cd.pmc->zmin)/10.0;
        cd.drawInfo.transZ -= dz;
    }
    else if(cd.drawInfo.projType==-1){
        double dx=(cd.pmc->xmax - cd.pmc->xmin)/10.0;
        cd.drawInfo.orthDX += dx;
    }
    ui->openGLWidget->repaint();
}

void MainWindow::on_zoomIn_clicked()
{
    if(cd.drawInfo.projType==1){
        double dz=(cd.pmc->zmax - cd.pmc->zmin)/10.0;
        cd.drawInfo.transZ += dz;
    }
    else if(cd.drawInfo.projType==-1){
        double dx=(cd.pmc->xmax - cd.pmc->xmin)/10.0;
        cd.drawInfo.orthDX -= dx;
    }
    ui->openGLWidget->repaint();

}

void MainWindow::on_actionPlot_triggered()
{
    plotW->show();
    plotW->freshPW();
}

void MainWindow::slot_repaint(){
    ui->openGLWidget->repaint();
}

void MainWindow::on_actionQuick_triggered()
{
    ui->actionQuick->setChecked(true);
    ui->actionQuick->setEnabled(false);
    ui->actionDetailed->setChecked(false);
    ui->actionDetailed->setEnabled(true);
}

void MainWindow::on_actionDetailed_triggered()
{
    ui->actionQuick->setChecked(false);
    ui->actionQuick->setEnabled(true);
    ui->actionDetailed->setChecked(true);
    ui->actionDetailed->setEnabled(false);
}
