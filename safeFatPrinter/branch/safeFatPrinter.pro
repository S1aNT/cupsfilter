# -------------------------------------------------
# Project created by QtCreator 2009-10-30T11:20:27
# -------------------------------------------------
QT += network
CONFIG += warn_on \
    qt \
    console
TARGET = safeFatPrinter
TEMPLATE = app
SOURCES += main.cpp \
    startdlg.cpp \
    dcontroller.cpp \
    asktheuser.cpp
HEADERS += startdlg.h \
    dcontroller.h \
    tech_global.h \
    asktheuser.h
FORMS += startdlg.ui \
    asktheuser.ui
