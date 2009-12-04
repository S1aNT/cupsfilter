TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../interfaces \
    ../global
HEADERS = auth.h \
    ../interfaces/auth_plugin.h \
    ../global/tech_global.h
SOURCES = auth.cpp
TARGET = auth_plugin
DESTDIR = ../safeFatPrinter/plugins
OTHER_FILES += ../tmpl_plugin/templates.txt
