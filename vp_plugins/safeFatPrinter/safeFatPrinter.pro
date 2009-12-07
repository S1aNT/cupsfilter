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
    firstask.h \
    getusernamemandatdlg.h \
    ../global/config.h \
    ../global/tech_global.h \
    selectwindow.h \
    workfield.h \
    addelemdlg.h \
    previewwnd.h \
    addtemplate.h \
    teditor.h \
    view.h
SOURCES += main.cpp \
    mediator.cpp \
    firstask.cpp \
    getusernamemandatdlg.cpp \
    selectwindow.cpp \
    workfield.cpp \
    addelemdlg.cpp \
    previewwnd.cpp \
    addtemplate.cpp \
    teditor.cpp \
    view.cpp
FORMS += firstask.ui \
    getusernamemandatdlg.ui \
    selectwindow.ui \
    workfield.ui \
    addelemdlg.ui \
    previewwnd.ui \
    addtemplate.ui \
    teditor.ui
RESOURCES += images.qrc
TRANSLATIONS = safeFatPrinter_ru.ts
