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
    src/CmdParser.cpp

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
    Include/openni/XnVersion.h \
    Include/openni/XnUtils.h \
    Include/openni/XnUSBDevice.h \
    Include/openni/XnUSB.h \
    Include/openni/XnTypes.h \
    Include/openni/XnThreadSafeQueue.h \
    Include/openni/XnStringsHash.h \
    Include/openni/XnStatusRegister.h \
    Include/openni/XnStatusCodes.h \
    Include/openni/XnStatus.h \
    Include/openni/XnStack.h \
    Include/openni/XnScheduler.h \
    Include/openni/XnQueue.h \
    Include/openni/XnQueries.h \
    Include/openni/XnPropNames.h \
    Include/openni/XnProfiling.h \
    Include/openni/XnPrdNodeInfoList.h \
    Include/openni/XnPrdNodeInfo.h \
    Include/openni/XnPrdNode.h \
    Include/openni/XnPlatform.h \
    Include/openni/XnOSStrings.h \
    Include/openni/XnOSMemory.h \
    Include/openni/XnOSCpp.h \
    Include/openni/XnOS.h \
    Include/openni/XnOpenNI.h \
    Include/openni/XnNodeAllocator.h \
    Include/openni/XnNode.h \
    Include/openni/XnModuleInterface.h \
    Include/openni/XnModuleCppRegistratration.h \
    Include/openni/XnModuleCppInterface.h \
    Include/openni/XnModuleCFunctions.h \
    Include/openni/XnMacros.h \
    Include/openni/XnLogWriterBase.h \
    Include/openni/XnLogTypes.h \
    Include/openni/XnLog.h \
    Include/openni/XnList.h \
    Include/openni/XnLicensing.h \
    Include/openni/XnInternalDefs.h \
    Include/openni/XnHash.h \
    Include/openni/XnGeneralBuffer.h \
    Include/openni/XnFPSCalculator.h \
    Include/openni/XnEvent.h \
    Include/openni/XnEnumerationErrors.h \
    Include/openni/XnDumpWriters.h \
    Include/openni/XnDump.h \
    Include/openni/XnDerivedCast.h \
    Include/openni/XnDataTypes.h \
    Include/openni/XnCppWrapper.h \
    Include/openni/XnContext.h \
    Include/openni/XnCodecIDs.h \
    Include/openni/XnCallback.h \
    Include/openni/XnBitSet.h \
    Include/openni/XnBaseNode.h \
    Include/openni/XnArray.h \
    Include/openni/XnAlgorithms.h \
    Include/openni/IXnNodeAllocator.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/openni

unix:!symbian|win32: LIBS += -L$$PWD/lib/ -llibcurl
unix:!symbian|win32: LIBS += -L$$PWD/lib/ -lopenNI
unix:!symbian|win32: LIBS += -L$$PWD/lib/ -llibexpat

