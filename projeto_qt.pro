TARGET = projeto_qt
TEMPLATE = app

CONFIG += debug

QT += widgets

HEADERS += Membro.h \
            Corpo.h \
            ComponenteOpenGL.h

SOURCES += Membro.cpp \
            Corpo.cpp \
            ComponenteOpenGL.cpp \
            main.cpp
