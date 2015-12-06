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
    gamebryodataarchives.cpp \
    gamebryoscriptextender.cpp \
    gamebryosavegame.cpp

HEADERS += gamegamebryo.h \
    dummybsa.h \
    gamebryobsainvalidation.h \
    gamebryodataarchives.h \
    gamebryoscriptextender.h \
    gamebryosavegame.h

include(../plugin_template.pri)

INCLUDEPATH +=  "$${BOOSTPATH}" "$${PWD}/../gamefeatures"

OTHER_FILES +=\
    SConscript \
    CMakeLists.txt
