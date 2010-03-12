QT += sql \
    network
TEMPLATE = app
TARGET = auth_net_server
CONFIG += warn_on \
    qt \
    console \
    precompile_header
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
LANGUAGE = C++

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../global/pre_header.h
DEPENDPATH += .
INCLUDEPATH += ../interfaces \
    ../global
HEADERS = server.h \
          servergears.h     \
    ../interfaces/auth_plugin.h \
    ../interfaces/inet_plugin.h  \
    ../global/mytypes.h
SOURCES = server.cpp \
    main.cpp \
    servergears.cpp
RESOURCES = images.qrc
FORMS += form.ui
