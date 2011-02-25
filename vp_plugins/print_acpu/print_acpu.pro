TEMPLATE = app
QT += gui \
    sql \
    network
CONFIG += warn_on \
    qt \
    precompile_header
INCLUDEPATH += ../../interfaces \
    ../../global
TARGET = print_acpu
win32:DESTDIR = c:/opt/vprn
else:DESTDIR = ~/bin
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
    mainwindow.h \
    filetransmitter.h \
    msgsockettransmitter.h \
    sqlmanager.h
SOURCES = main.cpp \
    mainwindow.cpp \
    message.cpp \
    filetransmitter.cpp \
    msgsockettransmitter.cpp \
    sqlmanager.cpp
FORMS += mainwindow.ui
