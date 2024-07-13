cmake_minimum_required(VERSION 3.16)

add_library(game_skyrimse SHARED
	gameskyrimse.cpp
	gameskyrimse.h
	skyrimsedataarchives.cpp
	skyrimsedataarchives.h
	skyrimsemoddatachecker.h
	skyrimsemoddatacontent.h
	skyrimsesavegame.cpp
	skyrimsesavegame.h
	skyrimsescriptextender.cpp
	skyrimsescriptextender.h
	skyrimseunmanagedmods.cpp
	skyrimseunmanagedmods.h
)
mo2_configure_plugin(game_skyrimse NO_SOURCES WARNINGS 4)
mo2_default_source_group()
target_link_libraries(game_skyrimse PRIVATE game_creation)
mo2_install_plugin(game_skyrimse)
