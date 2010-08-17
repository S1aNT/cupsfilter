TEMPLATE = lib
QT += gui
TARGET = gs_plugin
CONFIG += plugin \
    warn_on \
    precompile_header
win32 {
DESTDIR = c:/opt/vprn/plugins
}else{
DESTDIR =/opt/vprn/plugins
}
CONFIG(debug, debug|release) { 
    message(Build Debug!)
    RCC_DIR = ../build/$${TARGET}/debug/rcc
    MOC_DIR = ../build/$${TARGET}/debug/moc
    OBJECTS_DIR = ../build/$${TARGET}/debug/obj
    mac|unix:TARGET = $$join(TARGET,,,_debug)
    win32:TARGET = $$join(TARGET,,d)
}
else { 
    message(Build Release!)
    RCC_DIR = ../build/$${TARGET}/release/rcc
    MOC_DIR = ../build/$${TARGET}/release/moc
    OBJECTS_DIR = ../build/$${TARGET}/release/obj
}
INCLUDEPATH += ../interfaces \
    ../global
LANGUAGE = C++

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../global/pre_header.h
HEADERS += ../global/pre_header.h \
    gs_plugin.h \
    ../interfaces/igs_plugin.h \
    ../global/mytypes.h \
    proc_thread.h \
    clientdata.h
SOURCES = gs_plugin.cpp \
    proc_thread.cpp \
    clientdata.cpp
