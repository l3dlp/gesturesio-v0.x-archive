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
    src/NITcpServer.cpp \
    src/CmdParser.cpp \
    initworker.cpp

HEADERS  += mainwindow.h \
    Include/Utils.h \
    Include/NIEngine.h \
    Include/TinyThread/tinythread.h \
    Include/vjason/json.h \
    Include/vjason/block_allocator.h \
    Include/NIServer.h \
    Include/NITcpServer.h \
    Include/CmdParser.h \
    Include/OneEuroFilter.h \
    Include/curl/typecheck-gcc.h \
    Include/curl/stdcheaders.h \
    Include/curl/multi.h \
    Include/curl/mprintf.h \
    Include/curl/easy.h \
    Include/curl/curlver.h \
    Include/curl/curlrules.h \
    Include/curl/curlbuild.h \
    Include/curl/curl.h \
    Include/expat/expat_external.h \
    Include/expat/expat.h \
    Include/nite/NiteVersion.h \
    Include/nite/NiteEnums.h \
    Include/nite/NiteCTypes.h \
    Include/nite/NiteCEnums.h \
    Include/nite/NiteCAPI.h \
    Include/nite/NiTE.h \
    Include/openni/PS1080.h \
    Include/openni/OpenNI.h \
    Include/openni/OniVersion.h \
    Include/openni/OniProperties.h \
    Include/openni/OniPlatform.h \
    Include/openni/OniEnums.h \
    Include/openni/OniCTypes.h \
    Include/openni/OniCProperties.h \
    Include/openni/OniCEnums.h \
    Include/openni/OniCAPI.h \
    initworker.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/openni
INCLUDEPATH += $$PWD/include/nite

unix:!symbian|win32: LIBS += -L$$PWD/lib/ -llibcurl
unix:!symbian|win32: LIBS += -L$$PWD/lib/ -lOpenNI2
unix:!symbian|win32: LIBS += -L$$PWD/lib/ -llibexpat
unix:!symbian|win32: LIBS += -L$$PWD/lib/ -lNiTE2
