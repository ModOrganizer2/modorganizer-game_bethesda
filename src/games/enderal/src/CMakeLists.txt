cmake_minimum_required(VERSION 3.16)

add_library(game_enderal SHARED)
mo2_configure_plugin(game_enderal WARNINGS OFF)
target_link_libraries(game_enderal PRIVATE game_creation)
mo2_install_plugin(game_enderal)

