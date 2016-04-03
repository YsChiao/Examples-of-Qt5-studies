#-------------------------------------------------
#
# Project created by QtCreator 2016-04-02T18:30:22
#
#-------------------------------------------------

QT       += core gui multimedia xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtGame01
TEMPLATE = app


SOURCES += main.cpp\
        game.cpp \
    player.cpp \
    health.cpp \
    scores.cpp \
    bullet.cpp \
    enemy.cpp \
    resource.cpp

HEADERS  += game.h \
    player.h \
    health.h \
    scores.h \
    bullet.h \
    enemy.h \
    resource.h

RESOURCES += \
    resource.qrc
