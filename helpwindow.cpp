#include "helpwindow.h"
#include "ui_helpwindow.h"
#include <QFile>
#include <iostream>
using namespace std;

HelpWindow::HelpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HelpWindow)
{
    ui->setupUi(this);
    QFile file(":/help/help.html");
    file.open(QIODevice::ReadOnly);
    //string str=file.readAll().toStdString();

    ui->helpBR->insertHtml(file.readAll());
    file.close();

    QTextCursor cursor=ui->helpBR->textCursor();
    cursor.setPosition(0);
    ui->helpBR->setTextCursor(cursor);

}

HelpWindow::~HelpWindow()
{
    delete ui;
}
