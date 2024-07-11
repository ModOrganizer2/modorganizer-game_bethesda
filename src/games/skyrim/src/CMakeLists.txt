cmake_minimum_required(VERSION 3.16)

add_library(game_skyrim SHARED)
mo2_configure_plugin(game_skyrim)
target_link_libraries(game_skyrim PRIVATE game_gamebryo Version)
mo2_install_plugin(game_skyrim)
