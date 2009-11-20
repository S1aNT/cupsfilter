QT += network
CONFIG += warn_on \
    qt \
    console
TEMPLATE = app
INCLUDEPATH += ../interfaces
TARGET = safeFatPrinter
SOURCES += main.cpp \
    mainwindow.cpp \
    mediator.cpp
HEADERS += mainwindow.h \
    ../interfaces/inet_plugin.h \
    mediator.h
FORMS += mainwindow.ui
OTHER_FILES += 
