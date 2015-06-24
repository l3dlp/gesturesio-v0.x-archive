#-------------------------------------------------
#
# Project created by QtCreator 2012-08-12T10:08:53
#
#-------------------------------------------------

QT       += core gui

TARGET = MSHandTracker
TEMPLATE = app

INCLUDEPATH += "C:/Program Files (x86)/OpenNI/Include"
INCLUDEPATH += "C:/Program Files (x86)/PrimeSense/NITE/Include"
LIBS += -L"C:/Program Files (x86)/OpenNI/Lib" -L"C:/Program Files (x86)/PrimeSense/NITE/Lib"
LIBS += -lOpenNI -lXnVNite_1_5_2

SOURCES += main.cpp \
    imagedisplay.cpp \
    mainwindow.cpp \
    indicator.cpp \
    devicesearcher.cpp

HEADERS  += \
    imagedisplay.h \
    mainwindow.h \
    indicator.h \
    devicesearcher.h

FORMS    += \
    imagedisplay.ui \
    mainwindow.ui \
    indicator.ui
