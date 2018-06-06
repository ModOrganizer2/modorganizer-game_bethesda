#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:36:33
#
#-------------------------------------------------


TARGET = gameFalloutTTW
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

DEFINES += GAMEFALLOUTTTW_LIBRARY

SOURCES += gamefalloutTTW.cpp \
    falloutttwbsainvalidation.cpp \
    falloutttwscriptextender.cpp \
    falloutttwdataarchives.cpp \
    falloutttwsavegame.cpp \
    falloutttwsavegameinfo.cpp

HEADERS += gamefalloutttw.h \
    falloutttwbsainvalidation.h \
    falloutttwscriptextender.h \
    falloutttwdataarchives.h \
    falloutttwsavegame.h \
    falloutttwsavegameinfo.h

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
    gamefalloutttw.json\
    SConscript \
    CMakeLists.txt

