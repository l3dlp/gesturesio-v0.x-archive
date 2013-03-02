#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T19:30:33
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = NIServer
TEMPLATE = app


SOURCES += ../../../Src/Utils.cpp \
    ../../../Src/ServerWorker.cpp \
    ../../../Src/NITcpServer.cpp \
    ../../../Src/NIServer.cpp \
    ../../../Src/NIEngine.cpp \
    ../../../Src/mainwindow.cpp \
    ../../../Src/main.cpp \
    ../../../Src/CmdParser.cpp \
    ../../../ThirdParty/Src/TinyThread/tinythread.cpp \
    ../../../ThirdParty/Src/vjson/json.cpp \
    ../../../ThirdParty/Src/vjson/block_allocator.cpp

HEADERS  += ../../../Include/Utils.h \
    ../../../Include/ServerWorker.h \
    ../../../Include/OneEuroFilter.h \
    ../../../Include/NITcpServer.h \
    ../../../Include/NIServer.h \
    ../../../Include/NIEngine.h \
    ../../../Include/mainwindow.h \
    ../../../Include/CmdParser.h \
    ../../../ThirdParty/Include/expat/expat_external.h \
    ../../../ThirdParty/Include/expat/expat.h \
    ../../../ThirdParty/Include/curl/typecheck-gcc.h \
    ../../../ThirdParty/Include/curl/stdcheaders.h \
    ../../../ThirdParty/Include/curl/multi.h \
    ../../../ThirdParty/Include/curl/mprintf.h \
    ../../../ThirdParty/Include/curl/easy.h \
    ../../../ThirdParty/Include/curl/curlver.h \
    ../../../ThirdParty/Include/curl/curlrules.h \
    ../../../ThirdParty/Include/curl/curlbuild.h \
    ../../../ThirdParty/Include/curl/curl.h \
    ../../../ThirdParty/Include/nite/NiteVersion.h \
    ../../../ThirdParty/Include/nite/NiteEnums.h \
    ../../../ThirdParty/Include/nite/NiteCTypes.h \
    ../../../ThirdParty/Include/nite/NiteCEnums.h \
    ../../../ThirdParty/Include/nite/NiteCAPI.h \
    ../../../ThirdParty/Include/nite/NiTE.h \
    ../../../ThirdParty/Include/openni/PS1080.h \
    ../../../ThirdParty/Include/openni/OpenNI.h \
    ../../../ThirdParty/Include/openni/OniVersion.h \
    ../../../ThirdParty/Include/openni/OniProperties.h \
    ../../../ThirdParty/Include/openni/OniPlatform.h \
    ../../../ThirdParty/Include/openni/OniEnums.h \
    ../../../ThirdParty/Include/openni/OniCTypes.h \
    ../../../ThirdParty/Include/openni/OniCProperties.h \
    ../../../ThirdParty/Include/openni/OniCEnums.h \
    ../../../ThirdParty/Include/openni/OniCAPI.h \
    ../../../ThirdParty/Include/TinyThread/tinythread.h \
    ../../../ThirdParty/Include/vjason/json.h \
    ../../../ThirdParty/Include/vjason/block_allocator.h

FORMS    += ../../../Src/mainwindow.ui

INCLUDEPATH += ../../../Include
INCLUDEPATH += ../../../ThirdParty/Include
INCLUDEPATH += ../../../ThirdParty/Include/openni
INCLUDEPATH += ../../../ThirdParty/Include/nite

#DEPENDPATH += $$PWD/include

unix:!symbian|win32: LIBS += -L../../../ThirdParty/Lib -llibcurl
unix:!symbian|win32: LIBS += -L../../../ThirdParty/Lib -lOpenNI2
unix:!symbian|win32: LIBS += -L../../../ThirdParty/Lib -llibexpat
unix:!symbian|win32: LIBS += -L../../../ThirdParty/Lib -lNiTE2
