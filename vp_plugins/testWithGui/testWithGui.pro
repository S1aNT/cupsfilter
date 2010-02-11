QT += testlib \
    sql
CONFIG += warn_on \
    qt \
    console
TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += ../interfaces \
    ../test \
    ../global
HEADERS += ../interfaces/itmpl_sql_plugin.h \
    ../test/plugin_worker.h \
    addtemplate.h \
    ../global/tinfoeditmodel.h
SOURCES += main.cpp \
    ../test/plugin_worker.cpp \
    addtemplate.cpp \
    ../global/tinfoeditmodel.cpp
FORMS += addtemplate.ui
RESOURCES += ../tEditor/images.qrc
