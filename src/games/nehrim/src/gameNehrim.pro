#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:36:33
#
#-------------------------------------------------


TARGET = gameNehrim
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

DEFINES += GAMENEHRIM_LIBRARY

SOURCES += gamenehrim.cpp \
    nehrimbsainvalidation.cpp \
    nehrimscriptextender.cpp \
    nehrimdataarchives.cpp \
    nehrimsavegame.cpp \
    nehrimsavegameinfo.cpp

HEADERS += gamenehrim.h \
    nehrimbsainvalidation.h \
    nehrimscriptextender.h \
    nehrimdataarchives.h \
    nehrimsavegame.h \
    nehrimsavegameinfo.h

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
    gamenehrim.json\
    SConscript \
    CMakeLists.txt
