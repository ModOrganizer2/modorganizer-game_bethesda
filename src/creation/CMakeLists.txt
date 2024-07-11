cmake_minimum_required(VERSION 3.16)

find_package(lz4 CONFIG REQUIRED)

add_library(game_creation STATIC)
mo2_configure_target(game_creation WARNINGS OFF TRANSLATIONS ON)
target_link_libraries(game_creation PUBLIC game_gamebryo PRIVATE lz4::lz4)
target_include_directories(game_creation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(FILES $<TARGET_FILE:game_creation> DESTINATION lib)

