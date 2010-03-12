TEMPLATE = lib
QT -= gui
QT += network

CONFIG += plugin\
          warn_on \
	  precompile_header 

DESTDIR = ../VPrn/plugins

CONFIG(debug, debug|release) { 
    message(Build Debug!)
    RCC_DIR = ../build/$${TARGET}/debug/rcc
    MOC_DIR = ../build/$${TARGET}/debug/moc
    OBJECTS_DIR = ../build/$${TARGET}/debug/obj

    mac|unix: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,d)
}
else { 
    message(Build Release!)
    RCC_DIR = ../build/$${TARGET}/release/rcc
    MOC_DIR = ../build/$${TARGET}/release/moc
    OBJECTS_DIR = ../build/$${TARGET}/release/obj
}

INCLUDEPATH += ../interfaces \
	       ../global/
# Use Precompiled headers (PCH)
HEADERS += net_plugin.h \
    ../interfaces/inet_plugin.h \
    ../global/tech_global.h
SOURCES = net_plugin.cpp
