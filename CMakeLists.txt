cmake_minimum_required(VERSION 3.16)

if(DEFINED DEPENDENCIES_DIR)
	include(${DEPENDENCIES_DIR}/modorganizer_super/cmake_common/mo2.cmake)
else()
	include(${CMAKE_CURRENT_LIST_DIR}/cmake_common/mo2.cmake)
endif()

project(game_bethesda)

mo2_configure_extension()

file(GLOB game_directories
    RELATIVE ${CMAKE_CURRENT_LIST_DIR}
    LIST_DIRECTORIES TRUE
    "game_*")
list(REMOVE_ITEM game_directories "game_gamebryo")

# add this first
add_subdirectory(game_gamebryo)

foreach(game_directory ${game_directories})
    add_subdirectory(${game_directory})
endforeach()
