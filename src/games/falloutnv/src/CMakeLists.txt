cmake_minimum_required(VERSION 3.16)

add_library(game_falloutNV SHARED)
mo2_configure_plugin(game_falloutNV)
target_link_libraries(game_falloutNV PRIVATE game_gamebryo)
mo2_install_plugin(game_falloutNV)

