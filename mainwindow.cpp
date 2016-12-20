#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

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
    ui->toolBar->addAction(ui->actionPreview);
    ui->toolBar->addAction(ui->actionRun);
    ui->toolBar->addAction(ui->actionPause);
    ui->toolBar->addAction(ui->actionContent);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onObjCellClick(int row, int col){
    freshEleTW();
}

void MainWindow::onIonChangeTW(int row, int col){
    string str = ui->ionTW->item(row,col)->text().toStdString();
    stringstream ss; ss<<str;
    if(col==0){
        if(str.size()==0){
            qtdata.ions.erase(row + qtdata.ions.begin());
            freshIonTW();
            return;
        }
        else{
            qtdata.ions[row].name = str;
        }
    }
    else if(col==1){ ss>>qtdata.ions[row].Z; }
    else if(col==2){ ss>>qtdata.ions[row].M; }
    else if(col==3){ ss>>qtdata.ions[row].number; }
    else if(col==4){ ss>>qtdata.ions[row].x; }
    else if(col==5){ ss>>qtdata.ions[row].y; }
    else if(col==6){ ss>>qtdata.ions[row].z; }
    else if(col==7){ ss>>qtdata.ions[row].vx; }
    else if(col==8){ ss>>qtdata.ions[row].vy; }
    else if(col==9){ ss>>qtdata.ions[row].vz; }
    else if(col==10){ ss>>qtdata.ions[row].energy; }
}

void MainWindow::onObjChangeTW(int row, int col){
    if(ui->objTW->item(row,col)->text().size()==0){
        qtdata.objs.erase(qtdata.objs.begin() + row);
        freshObjTW();
    }
}

void MainWindow::onEleChangeTW(int row, int col){
    int oi = ui->objTW->currentRow();
    if(oi>=0 && oi<qtdata.objs.size()){
        string str = ui->eleTW->item(row,col)->text().toStdString();
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
        ui->objTW->setItem(i, 0, new QTableWidgetItem(qtdata.objs[i].objFile.c_str()));
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
        ui->eleTW->setItem(i, 0, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.objs[curR].elements[i].Z;
        ss>>str;
        ui->eleTW->setItem(i, 1, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.objs[curR].elements[i].M;
        ss>>str;
        ui->eleTW->setItem(i, 2, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.objs[curR].elements[i].density;
        ss>>str;
        ui->eleTW->setItem(i, 3, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.objs[curR].elements[i].fraction;
        ss>>str;
        ui->eleTW->setItem(i, 4, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.objs[curR].elements[i].disE;
        ss>>str;
        ui->eleTW->setItem(i, 5, new QTableWidgetItem(str.c_str()));
    }
}

void MainWindow::freshIonTW(){
    int li = qtdata.ions.size();
    ui->ionTW->setRowCount(li);
    for(int i=0; i<li; i++){
        stringstream ss;
        string str;
        ss<<qtdata.ions[i].name; ss>>str;
        ui->ionTW->setItem(i, 0, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].Z; ss>>str;
        ui->ionTW->setItem(i, 1, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].M; ss>>str;
        ui->ionTW->setItem(i, 2, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].number; ss>>str;
        ui->ionTW->setItem(i, 3, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].x; ss>>str;
        ui->ionTW->setItem(i, 4, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].y; ss>>str;
        ui->ionTW->setItem(i, 5, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].z; ss>>str;
        ui->ionTW->setItem(i, 6, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].vx; ss>>str;
        ui->ionTW->setItem(i, 7, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].vy; ss>>str;
        ui->ionTW->setItem(i, 8, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].vz; ss>>str;
        ui->ionTW->setItem(i, 9, new QTableWidgetItem(str.c_str()));
        ss.clear(); ss.str(""); ss<<qtdata.ions[i].energy; ss>>str;
        ui->ionTW->setItem(i, 10, new QTableWidgetItem(str.c_str()));
    }
}


void MainWindow::onAddObjBt(){
    QFileDialog *fd=new QFileDialog(this);
    fd->setFilter("objfile(*.obj)");
    fd->setViewMode(QFileDialog::List);
    QStringList flist;
    ui->objTW->setColumnWidth(0,200);
    if(fd->exec()==QDialog::Accepted){
        flist=fd->selectedFiles();
        for(int i=0; i<flist.size(); i++){
            QString fname = flist[i];
            qtdata.objs.push_back(QTObj(fname.toStdString()));
        }

        freshObjTW();
    }
    else{
        fd->close();
    }

}

void MainWindow::onAddEleBt(){
    int oi = ui->objTW->currentRow();
    if(oi>=0 && oi<qtdata.objs.size()){
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
    fd->setFilter("t3dfile(*.t3d)");
    fd->setViewMode(QFileDialog::List);
    QStringList flist;
    if(fd->exec()==QDialog::Accepted){
        flist=fd->selectedFiles();
        QString fname = flist[0];
        qtdata.loadInput(fname.toStdString());
        freshIonTW();
        freshObjTW();
        this->setWindowTitle(fname);
    }
    else{
        fd->close();
    }

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
        fd->setFilter("t3dfile(*.t3d)");
        fd->setViewMode(QFileDialog::List);
        if(fd->exec()==QDialog::Accepted){
            QStringList flist = fd->selectedFiles();
            QString name = flist[0];
            qtdata.filePath = name.toStdString();
            qtdata.saveInput();
        }
    }

    this->setWindowTitle(qtdata.filePath.c_str());
}





























