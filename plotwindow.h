#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include "mainwindow.h"
#include <QMainWindow>
#include "plotinfo.h"
#include <vector>

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
    void freshSD();
    void plotGraphe();

private slots:
    void on_leftSD_actionTriggered(int action);

    void on_rightSD_actionTriggered(int action);

    void on_addPlot_clicked();

    void on_sliceEd_textChanged(const QString &arg1);

    void on_plotBT_clicked();

private:
    Ui::PlotWindow *ui;

signals:
    void signal_repaint();
};

#endif // PLOTWINDOW_H
