TEMPLATE = lib
QT += gui \
    xml \
    sql

CONFIG += plugin \
    precompile_header
INCLUDEPATH += ../interfaces \
    ../global


TARGET = tmpl_sql_plugin

win32 {
DESTDIR = d:/opt/vprn/plugins
}else{
DESTDIR =/opt/vprn/plugins
}

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

LANGUAGE = C++
# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../global/pre_header.h
HEADERS += ../global/pre_header.h \
           ../global/tech_global.h \
           ../global/mytypes.h \
           ../global/tinfoeditmodel.h \
           ../global/myscene.h \
           ../global/mytextitem.h \
           ../global/commands.h \
           ../global/editpagesmodel.h \
           tmpl_sql_plugin.h 
SOURCES = tmpl_sql_plugin.cpp \
    ../global/tinfoeditmodel.cpp \
    ../global/myscene.cpp \
    ../global/mytextitem.cpp \
    ../global/editpagesmodel.cpp
