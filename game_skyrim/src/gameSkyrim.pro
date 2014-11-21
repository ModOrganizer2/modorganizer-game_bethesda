#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:36:33
#
#-------------------------------------------------

QT       -= gui

TARGET = gameSkyrim
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

DEFINES += GAMESKYRIM_LIBRARY

SOURCES += gameskyrim.cpp

HEADERS += gameskyrim.h

include(../plugin_template.pri)

INCLUDEPATH += "$${BOOSTPATH}"

LIBS += -ladvapi32

OTHER_FILES += \
    gameskyrim.json
