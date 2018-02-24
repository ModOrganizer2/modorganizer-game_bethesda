#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:36:33
#
#-------------------------------------------------


TARGET = gameMorrowind
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll
DEFINES += GAMEMORROWIND_LIBRARY

SOURCES += gamemorrowind.cpp \
    morrowindbsainvalidation.cpp \
    morrowindscriptextender.cpp \
    morrowinddataarchives.cpp \
    morrowindsavegame.cpp \
    morrowindsavegameinfo.cpp \
	morrowindgameplugins.cpp

HEADERS += gamemorrowind.h \
    morrowindbsainvalidation.h \
    morrowindscriptextender.h \
    morrowinddataarchives.h \
    morrowindsavegame.h \
    morrowindsavegameinfo.h \
	morrowindgameplugins.h

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
    gamemorrowind.json\
    SConscript \
    CMakeLists.txt

