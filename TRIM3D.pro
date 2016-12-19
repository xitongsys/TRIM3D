#-------------------------------------------------
#
# Project created by QtCreator 2016-12-18T09:28:11
#
#-------------------------------------------------

QT       += core gui
LIBS += -lglut -lGL -lGLU

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TRIM3D
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    object3d.cpp \
    sample.cpp \
    atom.cpp \
    stopping.cpp \
    mc.cpp

HEADERS  += mainwindow.h \
    global.h \
    object3d.h \
    sample.h \
    atom.h \
    phyconst.h \
    stopping.h \
    mc.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    input.txt

