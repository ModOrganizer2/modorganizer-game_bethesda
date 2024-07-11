cmake_minimum_required(VERSION 3.16)

add_library(game_fallout76 SHARED)
mo2_configure_plugin(game_fallout76)
target_link_libraries(game_fallout76 PRIVATE game_creation)
mo2_install_plugin(game_fallout76)

