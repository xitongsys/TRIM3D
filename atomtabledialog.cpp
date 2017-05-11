#include "atomtabledialog.h"
#include "ui_atomtabledialog.h"
#include "mainwindow.h"

#include <sstream>
#include "datainfo.h"

using namespace std;

AtomTableDialog::AtomTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AtomTableDialog)
{
    ui->setupUi(this);
    loadAtomTable();

}

AtomTableDialog::~AtomTableDialog()
{
    delete ui;
}

void AtomTableDialog::loadAtomTable(){
    QFile atomTableFile(":/AtomList.txt");
    vector<string> nameList,ZList,massList,dispList;
    atomTableFile.open(QIODevice::ReadOnly);

    while(!atomTableFile.atEnd()){
        string line = atomTableFile.readLine().toStdString();
        stringstream ss; ss<<line;
        string atomName; int Z; double mass, disE;
        ss>>atomName>>Z>>mass>>disE;

        nameList.push_back(atomName);
        ss.clear(); ss.str(""); ss<<Z;
        ZList.push_back(""); ss>>ZList[ZList.size()-1];
        ss.clear(); ss.str(""); ss<<mass;
        massList.push_back(""); ss>>massList[massList.size()-1];
        ss.clear(); ss.str(""); ss<<disE;
        dispList.push_back(""); ss>>dispList[dispList.size()-1];
    }
    atomTableFile.close();
    int ln = nameList.size();
    ui->atomTableWidget->setRowCount(ln);
    for(int i=0; i<ln; i++){
        QString stmp; stmp=stmp.fromLocal8Bit(nameList[i].c_str());
        ui->atomTableWidget->setItem(i, 0, new QTableWidgetItem(stmp));

        stmp = stmp.fromLocal8Bit(ZList[i].c_str());
        ui->atomTableWidget->setItem(i, 1, new QTableWidgetItem(stmp));

        stmp = stmp.fromLocal8Bit(massList[i].c_str());
        ui->atomTableWidget->setItem(i, 2, new QTableWidgetItem(stmp));

        stmp = stmp.fromLocal8Bit(dispList[i].c_str());
        ui->atomTableWidget->setItem(i, 3, new QTableWidgetItem(stmp));
    }
}

void AtomTableDialog::on_atomTableWidget_cellDoubleClicked(int row, int column)
{
    this->accept();
}

void AtomTableDialog::on_atomTableWidget_cellClicked(int row, int column)
{
    string name=ui->atomTableWidget->item(row,0)->text().toStdString();
    string Zstr = ui->atomTableWidget->item(row,1)->text().toStdString();
    string massstr = ui->atomTableWidget->item(row,2)->text().toStdString();
    string dispstr = ui->atomTableWidget->item(row, 3)->text().toStdString();
    stringstream ss;
    ss.clear(); ss.str(""); ss<<Zstr; ss>>this->Z;
    ss.clear(); ss.str(""); ss<<massstr; ss>>this->mass;
    ss.clear(); ss.str(""); ss<<dispstr; ss>>this->disp;
    this->name = name;
}
