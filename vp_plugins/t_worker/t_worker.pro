QT += network \
    sql
CONFIG += warn_on \
    qt \
    console \
    precompile_header
TEMPLATE = app
INCLUDEPATH += ../interfaces \
    ../global
TARGET = tWorker
LANGUAGE = C++

win32:DESTDIR = d:/opt/vprn
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

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../global/pre_header.h
HEADERS += ../global/pre_header.h \
    ../interfaces/itmpl_sql_plugin.h \
    ../interfaces/igs_plugin.h \
    ../global/config.h \
    ../global/mytypes.h \
    ../global/message.h \
    mainwindow.h
SOURCES += main.cpp \
    ../global/message.cpp \
     mainwindow.cpp

RESOURCES += images.qrc

TRANSLATIONS = tworker_ru.ts
