cmake_minimum_required(VERSION 3.16)

add_library(game_fallout4vr SHARED)
mo2_configure_plugin(game_fallout4vr
    WARNINGS OFF
    PRIVATE_DEPENDS creation)
mo2_install_target(game_fallout4vr)
