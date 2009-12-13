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
    teditor.h \
    view.h 
SOURCES += main.cpp \
    addtemplate.cpp \
    teditor.cpp \
    view.cpp 
FORMS += getusernamemandatdlg.ui \
    addtemplate.ui \
    teditor.ui
RESOURCES += images.qrc
TRANSLATIONS = tEditor_ru.ts
