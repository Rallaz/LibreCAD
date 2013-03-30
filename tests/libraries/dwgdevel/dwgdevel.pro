#-------------------------------------------------
#
# Project created by QtCreator 2013-01-21T09:25:51
#
#-------------------------------------------------

QT       += core gui

GENERATED_DIR = ../../../generated/tests/libraries/dwgdevel
# Use common project definitions.
include(../../../settings.pro)
include(../../../common.pro)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dwgdevel
TEMPLATE = app

DEPENDPATH += \
    ../../../libraries/libdxfrw/src
INCLUDEPATH += $$DEPENDPATH

# QMAKE_CXXFLAGS_DEBUG += -Wold-style-cast

DEFINES += DRWG_DBG

PRE_TARGETDEPS += ../../../generated/lib/libdxfrw.a

# Additional libraries to load
LIBS += -L../../../generated/lib  \
    -ldxfrw

SOURCES += main.cpp\
        mainwindow.cpp \
        filterdxfrw.cpp

HEADERS  += mainwindow.h \
        filterdxfrw.h
