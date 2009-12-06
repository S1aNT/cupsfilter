TEMPLATE = lib
QT += gui
TARGET = tmpl_plugin
CONFIG(debug, debug|release) {
    message(Build Debug!)
    RCC_DIR = ../build/$${TARGET}/debug/rcc
    MOC_DIR = ../build/$${TARGET}/debug/moc
    OBJECTS_DIR = ../build/debug/obj

    mac|unix: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,d)
} else {
    message(Build Release!)
    RCC_DIR = ../build/$${TARGET}/release/rcc
    MOC_DIR = ../build/$${TARGET}/release/moc
    OBJECTS_DIR = ../build/release/obj
}
CONFIG += plugin \
	precompile_header

INCLUDEPATH += ../interfaces \
               ../global
DESTDIR = ../safeFatPrinter/plugins
LANGUAGE  = C++
# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = ../global/pre_header.h
HEADERS += ../global/pre_header.h \
	tmpl_plugin.h \
        simpleitem.h \
    ../interfaces/itmpl_plugin.h \
    ../global/tech_global.h
SOURCES = tmpl_plugin.cpp \
         simpleitem.cpp
