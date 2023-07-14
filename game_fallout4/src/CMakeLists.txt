cmake_minimum_required(VERSION 3.16)

add_library(game_fallout4 SHARED)
mo2_configure_plugin(game_fallout4
    WARNINGS OFF)
target_link_libraries(game_fallout4 PRIVATE game_creation)
mo2_install_target(game_fallout4)
