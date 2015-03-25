#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:36:33
#
#-------------------------------------------------


TARGET = gameFalloutNV
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

DEFINES += GAMEFALLOUTNV_LIBRARY

SOURCES += gamefalloutnv.cpp \
    falloutnvbsainvalidation.cpp \
    falloutnvscriptextender.cpp \
    falloutnvdataarchives.cpp

HEADERS += gamefalloutnv.h \
    falloutnvbsainvalidation.h \
    falloutnvscriptextender.h \
    falloutnvdataarchives.h

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
    gamefalloutnv.json\
    SConscript
