TEMPLATE = lib
QT += gui \
      xml \
      sql
TARGET = tmpl_sql_plugin
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
DESTDIR = ../test/plugins
LANGUAGE  = C++
# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = ../global/pre_header.h
HEADERS += ../global/pre_header.h \
        tmpl_sql_plugin.h

SOURCES = tmpl_sql_plugin.cpp
