#include "mainwindow.h"
#include <QApplication>
#include "sample.h"
#include <QMutex>
#include "logowindow.h"


QMutex mutexLock;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    MainWindow w;
    w.show();

    LogoWindow lw(&w);
    lw.show();

    return a.exec();
}

