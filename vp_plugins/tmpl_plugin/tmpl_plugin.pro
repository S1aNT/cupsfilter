TEMPLATE = lib
CONFIG += plugin
QT += gui
TARGET = tmpl_plugin
INCLUDEPATH += ../interfaces \
	       ../global	
DESTDIR = ../safeFatPrinter/plugins
HEADERS = tmpl_plugin.h \
    ../interfaces/itmpl_plugin.h \
    ../global/tech_global.h 
SOURCES = tmpl_plugin.cpp 
