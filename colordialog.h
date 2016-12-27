#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QDialog>
#include "mainwindow.h"

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

private slots:
    void on_bgBT_clicked();

    void on_ColorDialog_accepted();

    void on_atomZCB_activated(const QString &arg1);

    void on_atomColorBT_clicked();

    void on_structureBT_clicked();

    void on_applyBT_clicked();

    void on_bgASB_editingFinished();

    void on_objASB_editingFinished();

    void on_atomASB_editingFinished();

private:
    Ui::ColorDialog *ui;
};

#endif // COLORDIALOG_H
