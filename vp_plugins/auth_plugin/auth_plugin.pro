TEMPLATE        = lib
CONFIG         += plugin
INCLUDEPATH += ../interfaces
HEADERS         = auth.h \
		../interfaces/auth_plugin.h 
SOURCES         = auth.cpp
TARGET 		= auth_plugin

DESTDIR = ../safeFatPrinter/plugins
    

