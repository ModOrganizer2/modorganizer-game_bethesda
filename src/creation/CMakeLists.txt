cmake_minimum_required(VERSION 3.16)

project(game_creation)
set(project_type lib)
set(enable_warnings OFF)

include(../../../cmake_common/project.cmake)
include(../../../cmake_common/src.cmake)

requires_project(game_gamebryo game_features)
