#-------------------------------------------------
#
# Project created by QtCreator 2015-03-20T21:07:59
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wizualizer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        mapviewer.cpp \
    filecontroller.cpp

HEADERS  += mainwindow.h \
    mapviewer.h \
    filecontroller.h

FORMS    += mainwindow.ui
