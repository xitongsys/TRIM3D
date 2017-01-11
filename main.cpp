#include "mainwindow.h"
#include <QApplication>
#include <QMutex>
#include "logowindow.h"
#include "datainfo.h"

QMutex mutexLock;
DataInfo cd;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    MainWindow w;
    w.show();

    LogoWindow lw(&w);
    lw.show();

    return a.exec();
}

