TEMPLATE = lib
CONFIG += plugin
QT -= gui
QT += network
TARGET = net_plugin
INCLUDEPATH += ../interfaces
DESTDIR = ../safeFatPrinter/plugins
HEADERS = net_plugin.h \
    ../interfaces/inet_plugin.h 
SOURCES = net_plugin.cpp 
