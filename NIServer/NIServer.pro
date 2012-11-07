#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T19:30:33
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = NIServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    src/Utils.cpp \
    src/NIEngine.cpp \
    src/TinyThread/tinythread.cpp \
    src/vjson/json.cpp \
    src/vjson/block_allocator.cpp \
    src/NIServer.cpp \
    src/NITcpServer.cpp

HEADERS  += mainwindow.h \
    Include/Utils.h \
    Include/NIEngine.h \
    Include/TinyThread/tinythread.h \
    Include/vjason/json.h \
    Include/vjason/block_allocator.h \
    Include/NIServer.h \
    Include/NITcpServer.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/openni

unix:!symbian|win32: LIBS += -L$$PWD/lib/ -llibcurl
unix:!symbian|win32: LIBS += -L$$PWD/lib/ -lopenNI
unix:!symbian|win32: LIBS += -L$$PWD/lib/ -llibexpat

