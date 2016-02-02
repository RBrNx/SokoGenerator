#-------------------------------------------------
#
# Project created by QtCreator 2016-01-31T23:21:44
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SokoGenerator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sokogenerator.cpp \
    sokosolver.cpp

HEADERS  += mainwindow.h \
    sokogenerator.h \
    sokosolver.h

FORMS    += mainwindow.ui

win32: RC_ICONS = SokoGenerator.ico

RESOURCES += \
    textures.qrc
