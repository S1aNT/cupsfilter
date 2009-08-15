# -------------------------------------------------
# Project created by QtCreator 2009-08-14T11:38:20
# -------------------------------------------------
QT += opengl \
    testlib
TARGET = template_editor
TEMPLATE = app
SOURCES +=src\main.cpp \
    	  src\mainwindow.cpp \
    	  src\docview.cpp
HEADERS += src\mainwindow.h \
    	   src\docview.h
FORMS += ui\mainwindow.ui \
    	 ui\docview.ui

RESOURCES += images.qrc
