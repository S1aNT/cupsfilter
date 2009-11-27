QT += network
CONFIG += warn_on \
    qt \
    console
TEMPLATE = app
INCLUDEPATH += ../interfaces
TARGET = safeFatPrinter
SOURCES += main.cpp \
    mainwindow.cpp \
    mediator.cpp \
    firstask.cpp \
    getusernamemandatdlg.cpp
HEADERS += mainwindow.h \
    ../interfaces/inet_plugin.h \
    mediator.h \
    ../interfaces/config.h \
    firstask.h \
    getusernamemandatdlg.h \
    ../interfaces/tech_global.h
FORMS += mainwindow.ui \
    firstask.ui \
    getusernamemandatdlg.ui
RESOURCES += images.qrc
