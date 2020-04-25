cmake_minimum_required(VERSION 3.16)

project(game_fallout4)
set(project_type plugin)
set(enable_warnings OFF)

include(../cmake_common/project.cmake)
add_subdirectory(src)
