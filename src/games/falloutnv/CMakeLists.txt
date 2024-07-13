cmake_minimum_required(VERSION 3.16)

add_library(game_falloutnv SHARED
	falloutnvbsainvalidation.cpp
	falloutnvbsainvalidation.h
	falloutnvdataarchives.cpp
	falloutnvdataarchives.h
	falloutnvmoddatachecker.h
	falloutnvmoddatacontent.h
	falloutnvsavegame.cpp
	falloutnvsavegame.h
	falloutnvscriptextender.cpp
	falloutnvscriptextender.h
	gamefalloutnv.cpp
	gamefalloutnv.h
)
mo2_configure_plugin(game_falloutnv NO_SOURCES WARNINGS 4)
mo2_default_source_group()
target_link_libraries(game_falloutnv PRIVATE game_gamebryo)
mo2_install_plugin(game_falloutnv)
