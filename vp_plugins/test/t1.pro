QT += testlib \
      sql
CONFIG += warn_on \
    qt \
    console

TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += ../interfaces

HEADERS += ../interfaces/itmpl_sql_plugin.h \
	plugin_worker.h 
SOURCES += t1.cpp \
	plugin_worker.cpp
