cmake_minimum_required(VERSION 3.16)

project(game_gamebryo)
set(project_type lib)
set(enable_warnings OFF)

if(DEFINED DEPENDENCIES_DIR)
	include(${DEPENDENCIES_DIR}/modorganizer_super/cmake_common/project.cmake)
else()
	include(../cmake_common/project.cmake)
endif()
add_subdirectory(src/gamebryo)

# note that this also creates a project
add_subdirectory(src/creation)
