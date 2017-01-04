#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include <QTableWidgetItem>

namespace Ui {
class ColorDialog;
}

class ColorDialog : public QDialog
{
    Q_OBJECT

public:
    MainWindow *pp;

public:
    explicit ColorDialog(QWidget *parent = 0);
    ~ColorDialog();
    void freshPres();

private slots:
    void on_bgBT_clicked();

    void on_ColorDialog_accepted();

    void on_structureBT_clicked();

    void on_applyBT_clicked();

    void on_bgASB_editingFinished();

    void on_objASB_editingFinished();


    void on_addPresBT_clicked();

    void on_presWT_cellChanged(int row, int column);


    void on_presWT_cellClicked(int row, int column);

private:
    Ui::ColorDialog *ui;
};

#endif // COLORDIALOG_H
