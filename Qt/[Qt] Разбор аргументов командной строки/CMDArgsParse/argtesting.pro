#-------------------------------------------------
#
# Project created by QtCreator 2012-03-21T16:17:37
#
#-------------------------------------------------

QT       += core \
         += network \
         -= gui

TARGET = argtesting
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    opts.cpp \
    argsset.cpp \
    ojoerror.cpp

HEADERS += \
    opts.h \
    argsset.h \
    ojoerror.h \
    description.h
