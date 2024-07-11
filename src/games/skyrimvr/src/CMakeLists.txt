cmake_minimum_required(VERSION 3.16)

add_library(game_skyrimvr SHARED)
mo2_configure_plugin(game_skyrimvr)
target_link_libraries(game_skyrimvr PRIVATE game_creation)
mo2_install_plugin(game_skyrimvr)

