#-------------------------------------------------
#
# Project created by QtCreator 2013-01-15T18:16:42
#
#-------------------------------------------------

QT       += core gui sql

TARGET = test
TEMPLATE = app

MOC_DIR     += generated/mocs
UI_DIR      += generated/uis
RCC_DIR     += generated/rccs
OBJECTS_DIR += generated/objs

LIBS += \
    -L"C:/lib/" -lInitializationd \
    -L"C:/lib/" -lDbProcessd

INCLUDEPATH += \
    "C:/lib/include"

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
