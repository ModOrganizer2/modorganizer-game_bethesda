cmake_minimum_required(VERSION 3.16)

add_library(game_ttw SHARED)
mo2_configure_plugin(game_ttw)
target_link_libraries(game_ttw PRIVATE game_gamebryo)
mo2_install_plugin(game_ttw)

