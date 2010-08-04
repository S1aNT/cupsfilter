QT += sql \
      network
TEMPLATE += app
TARGET =p_monitor
CONFIG += warn_on \
          qt \        
          precompile_header
win32:DESTDIR = d:/opt/vprn
else:DESTDIR = /opt/vprn
CONFIG(debug, debug|release) {
    message(Build Debug!)
    RCC_DIR = ../build/$${TARGET}/debug/rcc
    MOC_DIR = ../build/$${TARGET}/debug/moc
    OBJECTS_DIR = ../build/$${TARGET}/debug/obj
    DESTDIR = $${DESTDIR}/debug
}
else {
    message(Build Release!)
    RCC_DIR = ../build/$${TARGET}/release/rcc
    MOC_DIR = ../build/$${TARGET}/release/moc
    OBJECTS_DIR = ../build/$${TARGET}/release/obj
    DESTDIR = $${DESTDIR}/release
}
LANGUAGE = C++

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../global/pre_header.h
DEPENDPATH += .
INCLUDEPATH += ../interfaces \
    ../global \
    ../qt_single_apps

include(../qt_single_apps/qtsingleapplication.pri)

HEADERS += mainwindow.h \
    ../global/mytypes.h \
    ../global/message.h \
    ../global/mysocketclient.h \
    intropage.h \
    checkdatapage.h \
    selectpage.h \
    printdatapage.h \
    previewpage.h \
    viewport.h \
    finishpage.h \
    datamodule.h \
    engine.h \
    getusernamemandatdlg.h
SOURCES += main.cpp \           
    mainwindow.cpp \
    ../global/message.cpp \
    ../global/mysocketclient.cpp \
    checkdatapage.cpp \
    intropage.cpp \
    selectpage.cpp \
    printdatapage.cpp \
    previewpage.cpp \
    viewport.cpp \
    finishpage.cpp \
    datamodule.cpp \ 
    engine.cpp \
    getusernamemandatdlg.cpp
RESOURCES = images.qrc
TRANSLATIONS = prn_monitor_ru.ts
