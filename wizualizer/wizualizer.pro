#-------------------------------------------------
#
# Project created by QtCreator 2015-03-20T21:07:59
#
#-------------------------------------------------

QT       += core gui opengl serialport
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wizualizer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        mapviewer.cpp \
    filecontroller.cpp \
    messagecontroller.cpp \
    robotcontroller.cpp \
    bluetoothcontroller.cpp \
    envobj.cpp \
    author.cpp \
    comwindow.cpp

HEADERS  += mainwindow.h \
    mapviewer.h \
    filecontroller.h \
    globalvariables.h \
    messagecontroller.h \
    robotcontroller.h \
    bluetoothcontroller.h \
    envobj.h \
    author.h \
    comwindow.h

FORMS    += mainwindow.ui \
    author.ui \
    comwindow.ui
