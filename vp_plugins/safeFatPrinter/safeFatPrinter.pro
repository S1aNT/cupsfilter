QT += network
CONFIG += warn_on \
    qt \
    precompile_header
TEMPLATE = app
INCLUDEPATH += ../interfaces \
    ../global
TARGET = safeFatPrinter
LANGUAGE = C++

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../global/pre_header.h
HEADERS += ../global/pre_header.h \
    mediator.h \
    getusernamemandatdlg.h \
    ../global/config.h \
    ../global/tech_global.h \
    selectwindow.h \
    workfield.h \
    previewwnd.h \
    view.h \
    ../global/templ_info.h \
    ../global/templ_info_d.h
SOURCES += main.cpp \
    mediator.cpp \
    getusernamemandatdlg.cpp \
    selectwindow.cpp \
    workfield.cpp \
    previewwnd.cpp \
    view.cpp \
    ../global/templ_info.cpp \
    ../global/templ_info_d.cpp
FORMS += getusernamemandatdlg.ui \
    selectwindow.ui \
    workfield.ui \
    previewwnd.ui
RESOURCES += images.qrc
TRANSLATIONS = safeFatPrinter_ru.ts
