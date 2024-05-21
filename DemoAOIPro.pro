#-------------------------------------------------
#
# Project created by QtCreator 2023-11-02T14:19:11
#
#-------------------------------------------------

QT       += core gui sql network serialport axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoAOIPro
TEMPLATE = app
#CONFIG += console

MOC_DIR = .\tmp
RCC_DIR = .\tmp
OBJECTS_DIR = .\tmp
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
QMAKE_CXXFLAGS_RELEASE = -Od -ZI -MD
QMAKE_LFLAGS_RELEASE = /DEBUG /INCREMENTAL:NO
QMAKE_CFLAGS += /utf-8
QMAKE_CXXFLAGS += /utf-8
DEFINES += UNICODE
DEFINES += UMBCS
QMAKE_CXXFLAGS -= -Zc:strictStrings

DEFINES += QSLOG_IS_SHARED_LIBRARY_IMPORT
DEFINES += QS_LOG_LINE_NUMBERS

INCLUDEPATH += .\include
INCLUDEPATH += .\httpheaders

SOURCES += \
        centralwgt.cpp \
        demoaoithread.cpp \
        gdatafactory.cpp \
        headinfowgt.cpp \
        httpcpps/httpconnectionhandler.cpp \
        httpcpps/httpconnectionhandlerpool.cpp \
        httpcpps/httpcookie.cpp \
        httpcpps/httpglobal.cpp \
        httpcpps/httplistener.cpp \
        httpcpps/httprequest.cpp \
        httpcpps/httprequesthandler.cpp \
        httpcpps/httpresponse.cpp \
        httpcpps/httpsession.cpp \
        httpcpps/httpsessionstore.cpp \
        httpcpps/staticfilecontroller.cpp \
        httpcpps/template.cpp \
        httpcpps/templatecache.cpp \
        httpcpps/templateloader.cpp \
        httpdemoaoimsg.cpp \
        httpmsgprocess.cpp \
        main.cpp \
        mainwindow.cpp \
        mesprocess.cpp

HEADERS += \
        CodeSoftPrinter.h \
        centralwgt.h \
        demoaoithread.h \
        gdatafactory.h \
        headinfowgt.h \
        httpdemoaoimsg.h \
        httpheaders/httpconnectionhandler.h \
        httpheaders/httpconnectionhandlerpool.h \
        httpheaders/httpcookie.h \
        httpheaders/httpglobal.h \
        httpheaders/httplistener.h \
        httpheaders/httprequest.h \
        httpheaders/httprequesthandler.h \
        httpheaders/httpresponse.h \
        httpheaders/httpsession.h \
        httpheaders/httpsessionstore.h \
        httpheaders/staticfilecontroller.h \
        httpheaders/template.h \
        httpheaders/templatecache.h \
        httpheaders/templateglobal.h \
        httpheaders/templateloader.h \
        httpmsgprocess.h \
        include/aoi_plclinkdll.h \
        include/aoi_plclinkdll_global.h \
        include/aoi_zebraprinterdll.h \
        include/aoi_zebraprinterdll_global.h \
        include/aoi_zebraprinterdll_global.h \
        mainwindow.h \
        mesprocess.h

#DEFINES += QSLOG_IS_SHARED_LIBRARY_IMPORT
#DEFINES += QS_LOG_LINE_NUMBERS
#DEFINES += HTTP_ONLY

LIBS += -L$$PWD\lib\ -lQsLog2
LIBS += -L$$PWD\lib\ -lAOI_ZebraPrinterDll
LIBS += -L$$PWD\lib\ -lAOI_PLCLinkDll
LIBS += -L$$PWD\lib\ -lModbusClientDll
LIBS += -L$$PWD\lib\ -lCodeSoftPrinter

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
