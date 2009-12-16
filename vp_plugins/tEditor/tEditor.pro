QT += network
CONFIG += warn_on \
    qt \
    precompile_header
TEMPLATE = app
INCLUDEPATH += ../interfaces \
    ../global
TARGET = tEditor
LANGUAGE = C++

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../global/pre_header.h
HEADERS += ../global/pre_header.h \
    ../global/config.h \
    ../global/tech_global.h \
    addtemplate.h \
    view.h \
    mainwindow.h \
    ../interfaces/itmpl_plugin.h \
    cmdframe.h \
    ../global/config.h
SOURCES += main.cpp \
    addtemplate.cpp \
    view.cpp \
    mainwindow.cpp \
    cmdframe.cpp
FORMS += addtemplate.ui
RESOURCES += images.qrc
TRANSLATIONS = tEditor_ru.ts
