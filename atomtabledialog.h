#ifndef ATOMTABLEDIALOG_H
#define ATOMTABLEDIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include <QTableWidgetItem>

namespace Ui {
class AtomTableDialog;
}

class AtomTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AtomTableDialog(QWidget *parent = 0);
    ~AtomTableDialog();
    void loadAtomTable();
    int Z;
    double mass,disp;
    string name;

private slots:
    void on_atomTableWidget_cellDoubleClicked(int row, int column);

    void on_atomTableWidget_cellClicked(int row, int column);

private:
    Ui::AtomTableDialog *ui;
};

#endif // ATOMTABLEDIALOG_H
