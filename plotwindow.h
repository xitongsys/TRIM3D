#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include "mainwindow.h"
#include <QMainWindow>
#include "plotinfo.h"

class MainWindow;


namespace Ui {
class PlotWindow;
}

class PlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = 0);
    ~PlotWindow();

    MainWindow *pp;

    void freshPW();

private:
    Ui::PlotWindow *ui;
};

#endif // PLOTWINDOW_H
