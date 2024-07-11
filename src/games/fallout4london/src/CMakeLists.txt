cmake_minimum_required(VERSION 3.16)

add_library(game_fo4london SHARED)
mo2_configure_plugin(game_fo4london)
target_link_libraries(game_fo4london PRIVATE game_creation)
mo2_install_plugin(game_fo4london)
