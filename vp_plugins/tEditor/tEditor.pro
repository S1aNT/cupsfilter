QT += network \
    sql
CONFIG += warn_on \
    qt \
    precompile_header
TEMPLATE = app
INCLUDEPATH += ../interfaces \
    ../global
TARGET = tEditor
LANGUAGE = C++

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

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../global/pre_header.h
HEADERS += ../global/pre_header.h \
    ../global/config.h \
    ../global/tech_global.h \
    ../global/config.h \
    ../global/mytypes.h \
    ../global/myscene.h \
    ../global/mytextitem.h \
    ../global/editpagesmodel.h \
    ../interfaces/itmpl_sql_plugin.h \
    addtemplate.h \
    view.h \
    mainwindow.h \
    cmdframe.h \
    undoframe.h \
    ../global/loc_client.h\
    ../global/message.h
SOURCES += main.cpp \
    ../global/myscene.cpp \
    ../global/mytextitem.cpp \
    ../global/editpagesmodel.cpp \
    addtemplate.cpp \
    view.cpp \
    mainwindow.cpp \
    cmdframe.cpp \
    undoframe.cpp \
    ../global/loc_client.cpp \
    ../global/message.cpp
FORMS += addtemplate.ui 
RESOURCES += images.qrc
TRANSLATIONS = tEditor_ru.ts
