TEMPLATE = lib
CONFIG += plugin
QT += gui
TARGET = gs_plugin
INCLUDEPATH += ../interfaces
DESTDIR = ../safeFatPrinter/plugins
HEADERS = gs_plugin.h \
    ../interfaces/igs_plugin.h 
SOURCES = gs_plugin.cpp 
