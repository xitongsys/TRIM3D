#-------------------------------------------------
#
# Project created by QtCreator 2016-12-18T09:28:11
#
#-------------------------------------------------

QT       += core gui opengl
QT	 += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TRIM3D
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    object3d.cpp \
    sample.cpp \
    atom.cpp \
    stopping.cpp \
    mc.cpp \
    glwt.cpp \
    threadcal.cpp \
    logo.cpp \
    colordialog.cpp

HEADERS  += mainwindow.h \
    global.h \
    object3d.h \
    sample.h \
    atom.h \
    phyconst.h \
    stopping.h \
    mc.h \
    qtdata.h \
    glwt.h \
    drawinfo.h \
    threadcal.h \
    logo.h \
    colordialog.h \
    geometry.h

FORMS    += mainwindow.ui \
    colordialog.ui

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    input.txt \
    res.rc

RC_FILE += res.rc

RESOURCES += \
    res.qrc

