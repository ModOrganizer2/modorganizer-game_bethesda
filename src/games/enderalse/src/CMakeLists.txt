cmake_minimum_required(VERSION 3.16)

add_library(game_enderalse SHARED)
mo2_configure_plugin(game_enderalse)
target_link_libraries(game_enderalse PRIVATE game_creation)
mo2_install_plugin(game_enderalse)

