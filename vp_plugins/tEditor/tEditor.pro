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
DESTDIR = ../safeFatPrinter/debug/
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
HEADERS += ../global/pre_header.h \
    ../global/config.h \
    ../global/tech_global.h \
    ../global/config.h \
    ../global/mytypes.h \
    ../global/myscene.h \
    ../global/mytextitem.h \
    ../interfaces/itmpl_sql_plugin.h \
    addtemplate.h \
    view.h \
    mainwindow.h \
    cmdframe.h \
    undoframe.h
SOURCES += main.cpp \
    ../global/myscene.cpp \
    ../global/mytextitem.cpp \
    addtemplate.cpp \
    view.cpp \
    mainwindow.cpp \
    cmdframe.cpp \
    undoframe.cpp
FORMS += addtemplate.ui 
RESOURCES += images.qrc
TRANSLATIONS = tEditor_ru.ts
