#-------------------------------------------------
#
# Project created by QtCreator 2015-03-20T21:07:59
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wizualizer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mapmonitorviewer.cpp

HEADERS  += mainwindow.h \
    mapmonitorviewer.h \
    globalheader.h

FORMS    += mainwindow.ui
