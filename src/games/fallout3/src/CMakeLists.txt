cmake_minimum_required(VERSION 3.16)

add_library(game_fallout3 SHARED)
mo2_configure_plugin(game_fallout3)
target_link_libraries(game_fallout3 PRIVATE game_gamebryo)
mo2_install_plugin(game_fallout3)

