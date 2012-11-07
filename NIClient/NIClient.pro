#-------------------------------------------------
#
# Project created by QtCreator 2012-11-07T20:36:14
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = NIClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    NITcpClient.cpp

HEADERS  += mainwindow.h \
    NITcpClient.h

FORMS    += mainwindow.ui
