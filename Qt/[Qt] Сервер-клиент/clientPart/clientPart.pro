#-------------------------------------------------
#
# Project created by QtCreator 2013-09-21T14:13:45
#
#-------------------------------------------------

QT       += core gui network

TARGET = client
TEMPLATE = app

MOC_DIR     += generated/mocs
UI_DIR      += generated/uis
RCC_DIR     += generated/rccs
OBJECTS_DIR += generated/objs

SOURCES += main.cpp\
        mainwindow.cpp \
    clientStuff.cpp

HEADERS  += mainwindow.h \
    clientStuff.h

FORMS    += mainwindow.ui
