#-------------------------------------------------
#
# Project created by QtCreator 2013-05-13T12:23:44
#
#-------------------------------------------------

QT       += core gui network

TEMPLATE = app

CONFIG(debug, debug|release) {
    TARGET = serverd
} else {
    TARGET = server
}

MOC_DIR     += generated/mocs
UI_DIR      += generated/uis
RCC_DIR     += generated/rccs
OBJECTS_DIR += generated/objs

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    serverStuff.cpp

HEADERS += \
    mainwindow.h \
    serverStuff.h

FORMS += \
    mainwindow.ui
