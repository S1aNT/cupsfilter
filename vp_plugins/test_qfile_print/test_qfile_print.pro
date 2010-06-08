#-------------------------------------------------
#
# Project created by QtCreator 2010-06-08T16:40:33
#
#-------------------------------------------------

QT       += core

QT       += gui

TARGET = simple_print
CONFIG   += console warn
CONFIG   -= app_bundle

win32{
   DESTDIR = c:/opt/vprn
}else{
   DESTDIR = /opt/vprn
}

TEMPLATE = app


SOURCES += main.cpp
