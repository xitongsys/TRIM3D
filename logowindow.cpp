#include "logowindow.h"
#include "ui_logowindow.h"

LogoWindow::LogoWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogoWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->move(parent->width()/2 - this->width()/2, parent->height()/2-this->height()/2);

    QTimer *timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(hideLogo()));

    timer->start(2000);

}

LogoWindow::~LogoWindow()
{
    delete ui;
}


void LogoWindow::hideLogo(){
    this->hide();
}
