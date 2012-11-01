#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T19:30:33
#
#-------------------------------------------------

QT       += core gui

TARGET = NIServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    src/Utils.cpp \
    src/NIEngine.cpp \
    src/TinyThread/tinythread.cpp \
    src/vjson/json.cpp \
    src/vjson/block_allocator.cpp

HEADERS  += mainwindow.h \
    Include/Utils.h \
    Include/NIEngine.h \
    Include/TinyThread/tinythread.h \
    Include/vjason/json.h \
    Include/vjason/block_allocator.h \
    Include/clsockets/StatTimer.h \
    Include/clsockets/SimpleSocket.h \
    Include/clsockets/PassiveSocket.h \
    Include/clsockets/Host.h \
    Include/clsockets/ActiveSocket.h

FORMS    += mainwindow.ui

unix:!symbian|win32: LIBS += -L$$PWD/lib/ -llibcurl

INCLUDEPATH += $$PWD/include/openni
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!symbian|win32: LIBS += -L$$PWD/lib/ -lopenNI

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!symbian|win32: LIBS += -L$$PWD/lib/ -llibexpat

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

INCLUDEPATH += $$PWD/include/clsockets
