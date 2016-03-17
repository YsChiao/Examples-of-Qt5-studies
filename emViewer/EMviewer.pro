#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T22:18:37
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EMviewer
TEMPLATE = app


SOURCES += main.cpp\
        emviewer.cpp \
    globject.cpp

HEADERS  += emviewer.h \
    globject.h

FORMS    += emviewer.ui

LIBS += -lopengl32 -lglu32

RESOURCES += \
    resource.qrc
