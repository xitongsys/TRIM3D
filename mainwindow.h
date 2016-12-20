#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mc.h"
#include "qtdata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MC *pmc;
    QTInputData qtdata;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void onAddObjBt();
    void onAddEleBt();
    void onAddIonBt();

    void freshObjTW();
    void freshEleTW();
    void freshIonTW();

    void onObjChangeTW(int,int);
    void onObjCellClick(int,int);
    void onEleChangeTW(int,int);
    void onIonChangeTW(int,int);

};

#endif // MAINWINDOW_H
