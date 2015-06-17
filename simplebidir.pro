#-------------------------------------------------
#
# Project created by QtCreator 2014-12-20T15:28:41
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simplebidir
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fileFormatterUtility.cpp \
    searchComparision.cpp \
    blockcreator.cpp \
    heap.cpp

HEADERS  += mainwindow.h \
    functions.h \
    structures.h \
    blockcreator.h \
    fileformatterutility.h \
    heap.h \
    searchcomparision.h \
    headers.h

FORMS    += mainwindow.ui
