#-------------------------------------------------
#
# Project created by QtCreator 2017-12-21T23:49:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GridLayout
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    printwidget.cpp

HEADERS  += widget.h \
    printwidget.h

FORMS    += widget.ui \
    printwidget.ui

CONFIG += c++11
