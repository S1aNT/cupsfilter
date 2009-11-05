# -------------------------------------------------
# Project created by QtCreator 2009-10-30T11:20:27
# -------------------------------------------------
QT += network
CONFIG += warn_on \
    qt \
    console
TARGET = safeFatPrinter
TEMPLATE = app

# Input
HEADERS += src/asktheuser.h \
    src/dcontroller.h \
    src/startdlg.h \
    src/tech_global.h \
    src/workreport.h \
    src/proc_thread.h \
    src/net_agent.h \
    src/Spyder.h \
    src/config.h
FORMS += ui/asktheuser.ui \
    ui/startdlg.ui
SOURCES += src/asktheuser.cpp \
    src/dcontroller.cpp \
    src/main.cpp \
    src/startdlg.cpp \
    src/workreport.cpp \
    src/proc_thread.cpp \
    src/net_agent.cpp \
    src/spyder.cpp
RESOURCES += images.qrc
