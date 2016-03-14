#-------------------------------------------------
#
# Project created by QtCreator 2016-03-02T11:36:56
#
#-------------------------------------------------

QT       += core gui\
         webkitwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = webContent
TEMPLATE = app


SOURCES += main.cpp\
        webcontent.cpp \
    loadcontent.cpp

HEADERS  += webcontent.h \
    loadcontent.h

FORMS    += webcontent.ui

RESOURCES +=
