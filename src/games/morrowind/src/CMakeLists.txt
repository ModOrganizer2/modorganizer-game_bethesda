cmake_minimum_required(VERSION 3.16)

add_library(game_morrowind SHARED)
mo2_configure_plugin(game_morrowind)
target_link_libraries(game_morrowind PRIVATE game_gamebryo)
mo2_install_plugin(game_morrowind)

