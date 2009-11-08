# -------------------------------------------------
# Project created by QtCreator 2009-10-30T11:20:27
# -------------------------------------------------
QT += network
CONFIG += warn_on \
    qt \
    console
TARGET = safeFatPrinter
TEMPLATE = app

# Input
HEADERS += src/asktheuser.h \
    src/dcontroller.h \
    src/startdlg.h \
    src/tech_global.h \
    src/workreport.h \
<<<<<<< .mine
    src/template_painter.h\
=======
    src/proc_thread.h \
    src/net_agent.h \
    src/spyder.h \
    src/view.h \
    src/simpleitem.h \
    src/config.h \
    src/templ_scene.h
>>>>>>> .r214
FORMS += ui/asktheuser.ui \
    ui/startdlg.ui \
    ui/view.ui
SOURCES += src/asktheuser.cpp \
    src/dcontroller.cpp \
    src/main.cpp \
    src/startdlg.cpp \
    src/workreport.cpp \
<<<<<<< .mine
    src/template_painter.cpp\

=======
    src/proc_thread.cpp \
    src/net_agent.cpp \
    src/view.cpp \
    src/simpleitem.cpp \
    src/spyder.cpp \
    src/templ_scene.cpp
>>>>>>> .r214
RESOURCES += images.qrc
