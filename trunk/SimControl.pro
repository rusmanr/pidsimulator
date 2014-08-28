#-------------------------------------------------
#
# Project created by QtCreator 2014-06-28T17:04:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimControl
TEMPLATE = app
QWT_ROOT = /home/rusman/Qt/projects/qwt-6.1.0
INCLUDEPATH += $${QWT_ROOT}/src

    LIBS      += -L$${QWT_ROOT}/lib
    LIBS      += -lqwt

SOURCES += main.cpp\
        mainwindow.cpp \
    sim.cpp

HEADERS  += mainwindow.h \
    sim.h

FORMS    += mainwindow.ui

#LIBS+=-L/usr/lib /usr/lib/libkdeui.so

#QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
