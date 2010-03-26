QT += sql \
    network
TEMPLATE += app
TARGET = p_monitor
CONFIG += warn_on \
    qt \
    console \
    precompile_header
DESTDIR = ../VPrn
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
HEADERS += ../global/mytypes.h \
    ../global/message.h \
    ../global/mysocketclient.h \ 
     engine.h \
    printmonitor.h \
    getusernamemandatdlg.h \
    wizardpages.h
SOURCES += main.cpp \
    ../global/message.cpp \
    ../global/mysocketclient.cpp \ 
    engine.cpp \
    printmonitor.cpp \
    getusernamemandatdlg.cpp \
    wizardpages.cpp
RESOURCES = images.qrc
FORMS += getusernamemandatdlg.ui
TRANSLATIONS = print_monitor_ru.ts 