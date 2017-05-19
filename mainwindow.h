#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mc.h"
#include "qtdata.h"
#include "threadcal.h"
#include "helpwindow.h"
#include "plotwindow.h"

class PlotWindow;
class ThreadCal;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    HelpWindow hw;
    PlotWindow *plotW;

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

    void slot_fresh(int curi, int curj);
    void slot_repaint();

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionSave_triggered();
    void on_actionLoad_triggered();
    void on_actionRun_triggered();
    void on_actionStop_triggered();
    void on_actionExport_data_triggered();
    void on_actionAbout_triggered();
    void on_ifShowCB_clicked();
    void on_actionColor_triggered();


    void on_actionContent_triggered();

    void on_actionAxesLine_triggered();

    void on_actionAxes_3D_triggered();

    void on_actionPerspective_triggered();

    void on_actionOrthographic_triggered();

    void on_zoomOut_clicked();

    void on_zoomIn_clicked();

    void on_actionPlot_triggered();

    void on_actionQuick_triggered();

    void on_actionDetailed_triggered();

public:
    void freshGL();
};

#endif // MAINWINDOW_H
