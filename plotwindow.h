#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include "mainwindow.h"
#include <QMainWindow>
#include "plotinfo.h"

class MainWindow;


namespace Ui {
class PlotWindow;
}

class PlotWindow : public QMainWindow, public PlotInfo
{
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = 0);
    ~PlotWindow();

    MainWindow *pp;

private:
    Ui::PlotWindow *ui;
};

#endif // PLOTWINDOW_H
