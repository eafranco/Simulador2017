#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T15:03:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPUSimulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    controlsimulador.cpp \
    proceso.cpp \
    consolaproceso.cpp \
    editfile.cpp \
    helpfile.cpp \
    statusmemory.cpp \
    asim.cpp \
    cpu.cpp \
    showmem.cpp

HEADERS  += mainwindow.h \
    controlsimulador.h \
    proceso.h \
    consolaproceso.h \
    instrucciones.h \
    editfile.h \
    helpfile.h \
    statusmemory.h \
    asim.h \
    cpu.h \
    showmem.h

FORMS    += mainwindow.ui \
    consolaproceso.ui \
    editfile.ui \
    helpfile.ui \
    statusmemory.ui \
    showmem.ui

RESOURCES +=
