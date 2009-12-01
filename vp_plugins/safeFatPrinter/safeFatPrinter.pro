QT += network
CONFIG += warn_on \
    qt
TEMPLATE = app
INCLUDEPATH += ../interfaces \
    ../global
TARGET = safeFatPrinter
SOURCES += main.cpp \
    mainwindow.cpp \
    mediator.cpp \
    firstask.cpp \
    getusernamemandatdlg.cpp \
    selectwindow.cpp
HEADERS += mainwindow.h \
    mediator.h \
    firstask.h \
    getusernamemandatdlg.h \
    ../interfaces/inet_plugin.h \
    ../global/config.h \
    ../global/tech_global.h \
    selectwindow.h
FORMS += mainwindow.ui \
    firstask.ui \
    getusernamemandatdlg.ui \
    selectwindow.ui
RESOURCES += images.qrc
