TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../interfaces \
    ../global
HEADERS = auth.h \
    ../interfaces/auth_plugin.h \
    ../global/tech_global.h \
    ../tmpl_plugin/addtemplate.h
SOURCES = auth.cpp \
    ../tmpl_plugin/addtemplate.cpp
TARGET = auth_plugin
DESTDIR = ../safeFatPrinter/plugins
OTHER_FILES += ../tmpl_plugin/templates.txt
FORMS += ../tmpl_plugin/addtemplate.ui
