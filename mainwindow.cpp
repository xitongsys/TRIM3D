#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include "colordialog.h"
#include "helpwindow.h"
#include "plotwindow.h"

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
    ui->toolBar->addAction(ui->actionLoad);
    ui->toolBar->addAction(ui->actionRun);
    ui->toolBar->addAction(ui->actionStop);
    ui->toolBar->addAction(ui->actionExport_data);
    ui->toolBar->addAction(ui->actionColor);
    ui->toolBar->addAction(ui->actionContent);
    ui->toolBar->addAction(ui->actionAbout);

    pmc = NULL;
    connect(&tc, SIGNAL(signal_fresh(int,int)), this, SLOT(slot_fresh(int,int)));
}

void MainWindow::slot_fresh(int curi, int curj){
    if(ui->ifShowCB->isChecked()){
        ui->openGLWidget->repaint();
    }
    int ionNum=0, recoilNum=0;
    ionNum = pmc->incidentNum;
    recoilNum = pmc->recoilNum;
    stringstream ss; ss<<ionNum;
    string str; ss>>str;
    QString stmp; stmp = stmp.fromLocal8Bit(str.c_str());
    ui->ionNumEd->setText(stmp);
    ss.clear(); ss.str("");
    ss<<(recoilNum); ss>>str;
    stmp=stmp.fromLocal8Bit(str.c_str());
    ui->reNumEd->setText(stmp);

    int sum=0, sumcur=0;
    for(int i=0; i<(int)pmc->ions.size(); i++){
        sum += pmc->ionNum[i];
        if(i<curi){
            sumcur += pmc->ionNum[i];
        }
    }
    sumcur += curj + 1;
    ui->progressBar->setValue((int)((float)sumcur/sum * 100));
    if(tc.isStop()){
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
            qtdata.ions.erase(row + qtdata.ions.begin());
            freshIonTW();
            return;
        }
        else{
            qtdata.ions[col].name = str;
        }
    }
    else if(row==1){ ss>>qtdata.ions[col].Z; }
    else if(row==2){ ss>>qtdata.ions[col].M; }
    else if(row==3){ ss>>qtdata.ions[col].number; }
    else if(row==4){ ss>>qtdata.ions[col].x; }
    else if(row==5){ ss>>qtdata.ions[col].y; }
    else if(row==6){ ss>>qtdata.ions[col].z; }
    else if(row==7){ ss>>qtdata.ions[col].vx; }
    else if(row==8){ ss>>qtdata.ions[col].vy; }
    else if(row==9){ ss>>qtdata.ions[col].vz; }
    else if(row==10){ ss>>qtdata.ions[col].energy; }
}

void MainWindow::onObjChangeTW(int row, int col){
    if(ui->objTW->item(row,col)->text().size()==0){
        qtdata.objs.erase(qtdata.objs.begin() + row);
        freshObjTW();
    }
    else{
        qtdata.objs[row].objFile=ui->objTW->item(row,col)->text().toLocal8Bit().toStdString();
    }
    this->on_actionLoad_triggered();
}

void MainWindow::onEleChangeTW(int row, int col){
    int oi = ui->objTW->currentRow();
    if(oi>=0 && oi<(int)qtdata.objs.size()){
        string str = ui->eleTW->item(row,col)->text().toLocal8Bit().toStdString();
        if(col==0){
            if(str.size()==0){
                qtdata.objs[oi].delEle(row);
                freshEleTW();
                return;
            }
            else{
                qtdata.objs[oi].elements[row].name = str;
            }
        }
        else if(col==1){
            stringstream ss; ss<<str;
            ss>>qtdata.objs[oi].elements[row].Z;
        }
        else if(col==2){
            stringstream ss; ss<<str;
            ss>>qtdata.objs[oi].elements[row].M;
        }
        else if(col==3){
            stringstream ss; ss<<str;
            ss>>qtdata.objs[oi].elements[row].density;
        }
        else if(col==4){
            stringstream ss; ss<<str;
            ss>>qtdata.objs[oi].elements[row].fraction;
        }
        else if(col==5){
            stringstream ss; ss<<str;
            ss>>qtdata.objs[oi].elements[row].disE;
        }
    }
}

void MainWindow::freshObjTW(){
    int lo = qtdata.objs.size();
    ui->objTW->setRowCount(lo);
    for(int i=0; i<lo; i++){
        QString stmp; stmp = stmp.fromLocal8Bit(qtdata.objs[i].objFile.c_str());
        ui->objTW->setItem(i, 0, new QTableWidgetItem(stmp));
    }
}

void MainWindow::freshEleTW(){
    int curR = ui->objTW->currentRow();
    int lo = qtdata.objs.size();
    if(curR<0 || curR>=lo){
        ui->eleTW->setRowCount(0);
        return;
    }

    int le = qtdata.objs[curR].elements.size();
    ui->eleTW->setRowCount(le);
    for(int i=0; i<le; i++){
        stringstream ss;
        string str;
        str=qtdata.objs[curR].elements[i].name;
        QString stmp; stmp = stmp.fromLocal8Bit(str.c_str());
        ui->eleTW->setItem(i, 0, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.objs[curR].elements[i].Z;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->eleTW->setItem(i, 1, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.objs[curR].elements[i].M;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->eleTW->setItem(i, 2, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.objs[curR].elements[i].density;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->eleTW->setItem(i, 3, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.objs[curR].elements[i].fraction;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->eleTW->setItem(i, 4, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.objs[curR].elements[i].disE;
        ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->eleTW->setItem(i, 5, new QTableWidgetItem(stmp));
    }
}

void MainWindow::freshIonTW(){
    int li = qtdata.ions.size();
    ui->ionTW->setColumnCount(li);
    for(int i=0; i<li; i++){
        stringstream ss;
        string str; QString stmp;
        ss<<qtdata.ions[i].name; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(0,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].Z; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(1,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].M; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(2,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].number; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(3,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].x; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(4,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].y; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(5,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].z; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(6,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].vx; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(7,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].vy; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(8,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].vz; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(9,i, new QTableWidgetItem(stmp));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].energy; ss>>str;
        stmp=stmp.fromLocal8Bit(str.c_str());
        ui->ionTW->setItem(10,i, new QTableWidgetItem(stmp));
    }
}


void MainWindow::onAddObjBt(){
    QFileDialog *fd=new QFileDialog(this);
    fd->setNameFilter("obj file(*.obj)");
    fd->setViewMode(QFileDialog::List);
    QStringList flist;
    ui->objTW->setColumnWidth(0,200);
    if(fd->exec()==QDialog::Accepted){
        flist=fd->selectedFiles();
        for(int i=0; i<flist.size(); i++){
            QString fname = flist[i];
            qtdata.objs.push_back(QTObj(fname.toLocal8Bit().toStdString()));
        }

        freshObjTW();
    }
    else{
        fd->close();
    }

}

void MainWindow::onAddEleBt(){
    int oi = ui->objTW->currentRow();
    if(oi>=0 && oi<(int)qtdata.objs.size()){
        qtdata.objs[oi].addEle(QTEle("H",1,1.0087,0.05,1.0,5));
        freshEleTW();
    }

}

void MainWindow::onAddIonBt(){
    qtdata.ions.push_back(QTIon("H", 1, 1.0087, 1, 0,0,0, 0,0,1, 1000));
    freshIonTW();
}

void MainWindow::on_actionNew_triggered(){
    qtdata.clear();
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
        qtdata.loadInput(fname.toLocal8Bit().toStdString());
        freshIonTW();
        freshObjTW();
        this->setWindowTitle(fname);
    }
    else{
        fd->close();
    }

    this->on_actionLoad_triggered();

}

void MainWindow::on_actionExit_triggered()
{
   exit(0);
}

void MainWindow::on_actionSave_triggered(){
    string path = qtdata.filePath;
    if(path.size()!=0){
        qtdata.saveInput();
    }
    else{
        QFileDialog *fd=new QFileDialog(this);
        fd->setWindowTitle("Save input file");
        fd->setNameFilter("trim3d file(*.t3d)");
        fd->setViewMode(QFileDialog::List);
        if(fd->exec()==QDialog::Accepted){
            QStringList flist = fd->selectedFiles();
            QString name = flist[0];
            qtdata.filePath = name.toLocal8Bit().toStdString();
            qtdata.saveInput();
        }
    }

    QString stmp; stmp=stmp.fromLocal8Bit(qtdata.filePath.c_str());
    this->setWindowTitle(stmp);

}

void MainWindow::on_actionLoad_triggered(){
    this->on_actionSave_triggered();
    if(pmc!=NULL){
        ui->openGLWidget->pmc = NULL;
        delete pmc;
    }
    if(this->qtdata.filePath.size()==0) return;

    pmc = new MC(qtdata.filePath, "SCOEF.88");
    ui->openGLWidget->pmc = pmc;
    //ui->openGLWidget->resetView();
    ui->openGLWidget->repaint();
}

void MainWindow::on_actionRun_triggered(){
    if(pmc==NULL) return;
    tc.stop();
    this->on_actionLoad_triggered();
    if(pmc->objs.size()<=0 || pmc->ions.size()<=0) return;
    for(int i=0; i<(int)pmc->objs.size();i++){
        if(pmc->objs[i].elements.size()<=0)return;
    }
    tc.load(pmc, ui->openGLWidget);
    tc.start();
    ui->actionRun->setDisabled(true);
}

void MainWindow::on_actionStop_triggered(){
    tc.stop();
    ui->actionRun->setEnabled(true);
}

void MainWindow::on_actionExport_data_triggered() {
    if(pmc==NULL) return;
    QFileDialog *fd=new QFileDialog(this);
    fd->setWindowTitle("Export data file");
    fd->setNameFilter("all(*.*)");
    fd->setViewMode(QFileDialog::List);
    if(fd->exec()==QDialog::Accepted){
        QStringList flist=fd->selectedFiles();
        QString name=flist[0];
        qtdata.saveExport(name.toLocal8Bit().toStdString(), pmc);
    }
}

void MainWindow::on_actionAbout_triggered(){
    QMessageBox message(this);
    message.setWindowTitle("About TRIM3D");
    message.setText("Trim3D v1.0\n\n developed by Zhang Xitong\nEmail: xitongsys@gmail.com\n");
    message.setIconPixmap(QPixmap(":/images/mainIcon/ico128x128.png"));
    message.exec();

}


void MainWindow::on_ifShowCB_clicked() {
    if(ui->ifShowCB->checkState()){
        ui->openGLWidget->ifshow=1;
    }
    else{
        ui->openGLWidget->ifshow=0;
    }
    ui->openGLWidget->repaint();
}

void MainWindow::on_actionColor_triggered(){
    pDrawInfo=(DrawInfo*)ui->openGLWidget;
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
    ui->openGLWidget->ifDrawAxesLine *=-1;
    ui->openGLWidget->repaint();
}

void MainWindow::on_actionAxes_3D_triggered()
{
    ui->openGLWidget->ifDrawAxes3D *= -1;
    ui->openGLWidget->repaint();
}

void MainWindow::on_actionPerspective_triggered()
{
    ui->openGLWidget->projType*=-1;
    ui->actionOrthographic->setChecked(false);
    ui->actionPerspective->setDisabled(true);
    ui->actionOrthographic->setEnabled(true);


    ui->openGLWidget->repaint();
}


void MainWindow::on_actionOrthographic_triggered()
{
    ui->openGLWidget->projType*=-1;
    ui->actionPerspective->setChecked(false);
    ui->actionPerspective->setEnabled(true);
    ui->actionOrthographic->setDisabled(true);

    ui->openGLWidget->repaint();
}

void MainWindow::on_zoomOut_clicked()
{
    if(ui->openGLWidget->projType==1){
        double dz=(pmc->zmax - pmc->zmin)/10.0;
        ui->openGLWidget->transZ -= dz;
    }
    else if(ui->openGLWidget->projType==-1){
        double dx=(pmc->xmax - pmc->xmin)/10.0;
        ui->openGLWidget->orthDX += dx;
    }
    ui->openGLWidget->repaint();
}

void MainWindow::on_zoomIn_clicked()
{
    if(ui->openGLWidget->projType==1){
        double dz=(pmc->zmax - pmc->zmin)/10.0;
        ui->openGLWidget->transZ += dz;
    }
    else if(ui->openGLWidget->projType==-1){
        double dx=(pmc->xmax - pmc->xmin)/10.0;
        ui->openGLWidget->orthDX -= dx;
    }
    ui->openGLWidget->repaint();

}

void MainWindow::on_actionPlot_triggered()
{
    plotW.show();
}
