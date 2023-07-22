cmake_minimum_required(VERSION 3.16)

# TODO: clean include directives
set(MO2_CMAKE_DEPRECATED_UIBASE_INCLUDE ON)

project(game_bethesda)

find_package(mo2-cmake CONFIG REQUIRED)
find_package(Qt6 COMPONENTS Widgets)

add_subdirectory(src/gamebryo)
add_subdirectory(src/creation)

mo2_add_subdirectories(
    FOLDER games
    GLOB src/games/*
)

install(
FILES
    ${CMAKE_CURRENT_LIST_DIR}/icon.png
    ${CMAKE_CURRENT_LIST_DIR}/metadata.json
DESTINATION bin/extensions/${CMAKE_PROJECT_NAME}/)
