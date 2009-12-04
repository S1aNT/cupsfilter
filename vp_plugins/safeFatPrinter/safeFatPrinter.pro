QT += network
CONFIG += warn_on \
    qt
TEMPLATE = app
INCLUDEPATH += ../interfaces \
    ../global
TARGET = safeFatPrinter
SOURCES += main.cpp \
    mediator.cpp \
    firstask.cpp \
    getusernamemandatdlg.cpp \
    selectwindow.cpp \
    workfield.cpp \
    addelemdlg.cpp \
    previewwnd.cpp
HEADERS += mediator.h \
    firstask.h \
    getusernamemandatdlg.h \
    ../global/config.h \
    ../global/tech_global.h \
    selectwindow.h \
    workfield.h \
    addelemdlg.h \
    previewwnd.h 
FORMS += firstask.ui \
    getusernamemandatdlg.ui \
    selectwindow.ui \
    workfield.ui \
    addelemdlg.ui \
    previewwnd.ui
RESOURCES += images.qrc
