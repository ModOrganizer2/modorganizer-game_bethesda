#-------------------------------------------------
#
# Project created by QtCreator 2015-01-26T19:47:42
#
#-------------------------------------------------

TARGET = gameGamebryo
TEMPLATE = lib
CONFIG += staticlib

SOURCES += gamegamebryo.cpp \
    dummybsa.cpp \
    gamebryobsainvalidation.cpp \
    gamebryodataarchives.cpp

HEADERS += gamegamebryo.h \
    dummybsa.h \
    gamebryobsainvalidation.h \
    gamebryodataarchives.h

include(../plugin_template.pri)

INCLUDEPATH +=  "$${BOOSTPATH}" "$${PWD}/../gamefeatures"
