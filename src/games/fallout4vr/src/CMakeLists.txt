cmake_minimum_required(VERSION 3.16)

add_library(game_fallout4vr SHARED)
mo2_configure_plugin(game_fallout4vr)
target_link_libraries(game_fallout4vr PRIVATE game_creation)
mo2_install_plugin(game_fallout4vr)

