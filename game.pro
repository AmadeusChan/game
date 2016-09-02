#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T10:28:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = game
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mynetwork.cpp \
    mygame.cpp \
    dialog.cpp \
    dialogforclient.cpp

HEADERS  += mainwindow.h \
    mynetwork.h \
    mygame.h \
    dialog.h \
    dialogforclient.h

FORMS    += mainwindow.ui \
    dialog.ui \
    dialogforclient.ui

QT += network

RESOURCES += \
    image.qrc
