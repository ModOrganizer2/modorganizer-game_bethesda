#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:36:33
#
#-------------------------------------------------


TARGET = gameFallout3
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

DEFINES += GAMEFALLOUTNV_LIBRARY

SOURCES += gamefallout3.cpp \
    fallout3bsainvalidation.cpp \
    fallout3scriptextender.cpp \
    fallout3dataarchives.cpp

HEADERS += gamefallout3.h \
    fallout3bsainvalidation.h \
    fallout3scriptextender.h \
    fallout3dataarchives.h

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
    gamefallout3.json\
    SConscript
