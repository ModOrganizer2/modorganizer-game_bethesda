cmake_minimum_required(VERSION 3.16)

find_package(mo2-uibase CONFIG REQUIRED)
find_package(lz4 CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

add_library(game_gamebryo STATIC
	dummybsa.cpp
	dummybsa.h
	gamebryobsainvalidation.cpp
	gamebryobsainvalidation.h
	gamebryodataarchives.cpp
	gamebryodataarchives.h
	gamebryogameplugins.cpp
	gamebryogameplugins.h
	gamebryolocalsavegames.cpp
	gamebryolocalsavegames.h
	gamebryomoddatachecker.cpp
	gamebryomoddatachecker.h
	gamebryomoddatacontent.cpp
	gamebryomoddatacontent.h
	gamebryosavegame.cpp
	gamebryosavegame.h
	gamebryosavegameinfo.cpp
	gamebryosavegameinfo.h
	gamebryosavegameinfowidget.cpp
	gamebryosavegameinfowidget.h
	gamebryosavegameinfowidget.ui
	gamebryoscriptextender.cpp
	gamebryoscriptextender.h
	gamebryounmanagedmods.cpp
	gamebryounmanagedmods.h
	gamegamebryo.cpp
	gamegamebryo.h
	vdf_parser.h
)
mo2_configure_target(game_gamebryo
	WARNINGS 4
	NO_SOURCES
	TRANSLATIONS ON
	EXTRA_TRANSLATIONS ${CMAKE_CURRENT_LIST_DIR}/../creation)
mo2_default_source_group()
target_link_libraries(game_gamebryo PUBLIC mo2::uibase PRIVATE ZLIB::ZLIB lz4::lz4 Dbghelp)
target_include_directories(game_gamebryo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(FILES $<TARGET_FILE:game_gamebryo> DESTINATION lib)
