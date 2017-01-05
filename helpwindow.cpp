#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HelpWindow)
{
    ui->setupUi(this);
    ui->helpBR->insertHtml("<b>hello,world</b>");
}

HelpWindow::~HelpWindow()
{
    delete ui;
}
