cmake_minimum_required(VERSION 3.16)

add_library(game_skyrimse SHARED)
mo2_configure_plugin(game_skyrimse
    WARNINGS OFF)
target_link_libraries(game_skyrimse PRIVATE game_creation)
mo2_install_target(game_skyrimse)
