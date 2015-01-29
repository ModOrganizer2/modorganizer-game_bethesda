#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:36:33
#
#-------------------------------------------------


TARGET = gameOblivion
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

DEFINES += GAMEOBLIVION_LIBRARY

SOURCES += gameoblivion.cpp \
    oblivionbsainvalidation.cpp \
    oblivionscriptextender.cpp \
    obliviondataarchives.cpp

HEADERS += gameoblivion.h \
    oblivionbsainvalidation.h \
    oblivionscriptextender.h \
    obliviondataarchives.h

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
    gameoblivion.json
