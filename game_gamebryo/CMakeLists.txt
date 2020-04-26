cmake_minimum_required(VERSION 3.16)

project(game_gamebryo)
set(project_type lib)
set(enable_warnings OFF)

include(../cmake_common/project.cmake)
add_subdirectory(src/gamebryo)

# note that this also creates a project
add_subdirectory(src/creation)
