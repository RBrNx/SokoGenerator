#-------------------------------------------------
#
# Project created by QtCreator 2016-01-31T23:21:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SokoGenerator
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    sokogenerator.cpp \
    sokosolver.cpp \
    boostsolver.cpp

HEADERS  += mainwindow.h \
    sokogenerator.h \
    sokosolver.h \
    boostsolver.h

FORMS    += mainwindow.ui

win32: RC_ICONS = SokoGenerator.ico

RESOURCES += \
    resources.qrc

INCLUDEPATH += "C:/Program Files (x86)/Java/jdk1.8.0_74/include/"
INCLUDEPATH += "C:/Program Files (x86)/Java/jdk1.8.0_74/include/win32/"
INCLUDEPATH += "C:/boost_1_60_0"
LIBS += "C:/Program Files (x86)/Java/jre1.8.0_74/bin/client/jvm.dll"
LIBS += -L"C:/Program Files (x86)/Java/jdk1.8.0_74/lib/" -ljvm

