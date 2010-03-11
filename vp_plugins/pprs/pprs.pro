QT += testlib \
      sql \
      network  
CONFIG += warn_on \
    qt \
    console

TEMPLATE = app
TARGET = pprs.exe

DESTDIR = ../Apps/debug/

CONFIG(debug, debug|release) { 
    message(Build Debug!)
    RCC_DIR = ../build/$${TARGET}/debug/rcc
    MOC_DIR = ../build/$${TARGET}/debug/moc
    OBJECTS_DIR = ../build/$${TARGET}/debug/obj
}
else { 
    message(Build Release!)
    RCC_DIR = ../build/$${TARGET}/release/rcc
    MOC_DIR = ../build/$${TARGET}/release/moc
    OBJECTS_DIR = ../build/$${TARGET}/release/obj
}

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../global/pre_header.h

DEPENDPATH += .
INCLUDEPATH += ../interfaces \
               ../global
HEADERS += ../global/mytypes.h \
           mygears.h \
           mainwindow.h
SOURCES += main.cpp \
           mygears.cpp \
           mainwindow.cpp
RESOURCES += images.qrc

