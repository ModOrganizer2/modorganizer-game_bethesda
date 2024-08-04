cmake_minimum_required(VERSION 3.16)

add_library(game_ttw SHARED)
mo2_configure_plugin(game_ttw
    WARNINGS OFF
    PRIVATE_DEPENDS gamebryo)
mo2_install_target(game_ttw)
