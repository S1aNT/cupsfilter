TEMPLATE = lib
CONFIG += plugin
QT += gui
TARGET = gs_plugin
INCLUDEPATH += ../interfaces \
	       ../global	
DESTDIR = ../safeFatPrinter/plugins
HEADERS = gs_plugin.h \
    ../interfaces/igs_plugin.h \
    ../global/tech_global.h \
    proc_thread.h 
SOURCES = gs_plugin.cpp \
    proc_thread.cpp
