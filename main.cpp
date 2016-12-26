#include "mainwindow.h"
#include <QApplication>
#include "sample.h"
#include <QMutex>

QMutex mutexLock;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

