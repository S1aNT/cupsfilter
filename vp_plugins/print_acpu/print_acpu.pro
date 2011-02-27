TEMPLATE = app
QT += gui \
    sql \
    network
CONFIG += warn_on \
    qt \
    precompile_header
INCLUDEPATH += ../interfaces \
    ../global
TARGET = print_acpu
win32:DESTDIR = c:/opt/vprn
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
HEADERS += mytypes.h \
    ../global/config.h \
    message.h \
    filetransmitter.h \
    msgsockettransmitter.h \
    sqlmanager.h \
    mainwidget.h \
maindialog.h \
    glass.h
SOURCES = main.cpp \
    message.cpp \
    filetransmitter.cpp \
    msgsockettransmitter.cpp \
    sqlmanager.cpp \
    mainwidget.cpp \
maindialog.cpp \
    glass.cpp

RESOURCES = images.qrc
TRANSLATIONS = print_acpu_ru.ts
