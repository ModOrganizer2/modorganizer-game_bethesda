#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T12:24:19
#
#-------------------------------------------------


TARGET = gameSkyrimVR
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

DEFINES += GAMESKYRIMVR_LIBRARY

SOURCES += gameskyrimvr.cpp \
    skyrimvrbsainvalidation.cpp \
    skyrimvrscriptextender.cpp \
    skyrimvrdataarchives.cpp \
    skyrimvrsavegame.cpp \
    skyrimvrsavegameinfo.cpp

HEADERS += gameskyrimvr.h \
    skyrimvrbsainvalidation.h \
    skyrimvrscriptextender.h \
    skyrimvrdataarchives.h \
    skyrimvrsavegame.h \
    skyrimvrsavegameinfo.h

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
    gameskyrimvr.json\
    SConscript \
    CMakeLists.txt

