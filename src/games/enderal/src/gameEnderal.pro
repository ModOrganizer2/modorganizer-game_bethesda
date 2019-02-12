#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:36:33
#
#-------------------------------------------------


TARGET = gameEnderal
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll
DEFINES += GAMESKYRIM_LIBRARY

SOURCES += gameenderal.cpp \
    enderalbsainvalidation.cpp \
    enderalscriptextender.cpp \
    enderaldataarchives.cpp \
    enderalsavegame.cpp \
    enderalsavegameinfo.cpp

HEADERS += gameenderal.h \
    enderalbsainvalidation.h \
    enderalscriptextender.h \
    enderaldataarchives.h \
    enderalsavegame.h \
    enderalsavegameinfo.h

CONFIG(debug, debug|release) {
  LIBS += -L"$${OUT_PWD}/../gameGamebryo/debug"
  PRE_TARGETDEPS += \
    $$OUT_PWD/../gameGamebryo/debug/gameGamebryo.lib
} else {
  LIBS += -L"$${OUT_PWD}/../gameGamebryo/release"
  PRE_TARGETDEPS += \
    $$OUT_PWD/../gameGamebryo/release/gameGamebryo.lib
}

include(../plugin_template.pri)

INCLUDEPATH += "$${BOOSTPATH}" "$${PWD}/../gamefeatures" "$${PWD}/../gamegamebryo"

LIBS += -ladvapi32 -lole32 -lgameGamebryo

OTHER_FILES += \
    gameenderal.json\
    SConscript \
    CMakeLists.txt

