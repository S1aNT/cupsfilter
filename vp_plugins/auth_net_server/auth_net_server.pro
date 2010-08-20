QT += sql \
    network
TEMPLATE += app
TARGET = auth_net_server
CONFIG += warn_on \
    qt \    
    precompile_header
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
DEPENDPATH += .
INCLUDEPATH += ../interfaces \
    ../global \
    ../qt_single_apps
include(../qt_single_apps/qtsingleapplication.pri)
HEADERS += server.h \
    servergears.h \
    ../interfaces/auth_plugin.h \
    ../interfaces/inet_plugin.h \
    ../interfaces/igs_plugin.h \
    ../interfaces/itmpl_sql_plugin.h \
    ../global/mytypes.h \
    ../global/message.h \
    ../global/templatesinfo.h \
    printtask.h
SOURCES += server.cpp \
    main.cpp \
    servergears.cpp \
    ../global/message.cpp \
    ../global/templatesinfo.cpp \
    printtask.cpp
RESOURCES = images.qrc
TRANSLATIONS = auth_net_server_ru.ts
FORMS += ../test_gui/form.ui
