#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:36:33
#
#-------------------------------------------------


TARGET = gameFallout3
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

DEFINES += GAMEFALLOUT76_LIBRARY

SOURCES += gamefallout76.cpp \
    fallout76bsainvalidation.cpp \
    fallout76scriptextender.cpp \
    fallout76dataarchives.cpp \
    fallout76savegame.cpp \
    fallout76savegameinfo.cpp

HEADERS += gamefallout76.h \
    fallout76bsainvalidation.h \
    fallout76scriptextender.h \
    fallout76dataarchives.h \
    fallout76savegame.h \
    fallout76savegameinfo.h

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
    gamefallout76.json\
    SConscript \
    CMakeLists.txt

