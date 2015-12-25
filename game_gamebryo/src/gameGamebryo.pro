#-------------------------------------------------
#
# Project created by QtCreator 2015-01-26T19:47:42
#
#-------------------------------------------------

TARGET = gameGamebryo
TEMPLATE = lib
CONFIG += staticlib

QT += widgets

SOURCES += gamegamebryo.cpp \
    dummybsa.cpp \
    gamebryobsainvalidation.cpp \
    gamebryodataarchives.cpp \
    gamebryoscriptextender.cpp \
    gamebryosavegame.cpp \
    gamebryosavegameinfo.cpp \
    gamebryosavegameinfowidget.cpp

HEADERS += gamegamebryo.h \
    dummybsa.h \
    gamebryobsainvalidation.h \
    gamebryodataarchives.h \
    gamebryoscriptextender.h \
    gamebryosavegame.h \
    gamebryosavegameinfo.h \
    gamebryosavegameinfowidget.h

include(../plugin_template.pri)

INCLUDEPATH +=  "$${BOOSTPATH}" "$${PWD}/../gamefeatures"

OTHER_FILES +=\
    SConscript \
    CMakeLists.txt

FORMS += \
    gamebryosavegameinfowidget.ui
