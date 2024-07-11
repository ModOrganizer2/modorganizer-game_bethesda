cmake_minimum_required(VERSION 3.16)

add_library(game_oblivion SHARED)
mo2_configure_plugin(game_oblivion)
target_link_libraries(game_oblivion PRIVATE game_gamebryo)
mo2_install_plugin(game_oblivion)

