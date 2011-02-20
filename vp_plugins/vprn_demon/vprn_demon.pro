# -------------------------------------------------
# Project local demon for virtual printer
# -------------------------------------------------
QT += network \
    sql
QT -= gui
TARGET = vprn_demon
CONFIG += console
CONFIG -= app_bundle
CONFIG += warn_on \
    precompile_header
TEMPLATE = app
win32:DESTDIR = c:/opt/vprn
else:DESTDIR = /opt/vprn
CONFIG(debug, debug|release) { 
    message(Build Debug!)
    RCC_DIR = build/$${TARGET}/debug/rcc
    MOC_DIR = build/$${TARGET}/debug/moc
    OBJECTS_DIR = build/$${TARGET}/debug/obj
    DESTDIR = $${DESTDIR}/debug
}
else { 
    message(Build Release!)
    RCC_DIR = build/$${TARGET}/release/rcc
    MOC_DIR = build/$${TARGET}/release/moc
    OBJECTS_DIR = build/$${TARGET}/release/obj
    DESTDIR = $${DESTDIR}/release
}
LANGUAGE = C++

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../global/pre_header.h
DEPENDPATH += .
INCLUDEPATH += ../global
HEADERS += ../global/config.h \
    ../global/message.h \
    ../global/mytypes.h \
    mysocketclient.h \
    servercore.h \
    datamodule.h
SOURCES += main.cpp \
    ../global/message.cpp \
    mysocketclient.cpp \
    servercore.cpp \
    datamodule.cpp
RESOURCES = images.qrc
TRANSLATIONS = vprn_demon.ts
