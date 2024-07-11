cmake_minimum_required(VERSION 3.16)

add_library(game_starfield SHARED)
mo2_configure_plugin(game_starfield)
target_link_libraries(game_starfield PRIVATE game_creation)
mo2_install_plugin(game_starfield)

