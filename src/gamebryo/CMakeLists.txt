cmake_minimum_required(VERSION 3.16)

find_package(mo2-uibase CONFIG REQUIRED)
find_package(lz4 CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

add_library(game_gamebryo STATIC)
mo2_configure_target(game_gamebryo WARNINGS OFF TRANSLATIONS ON AUTOMOC ON)
target_link_libraries(game_gamebryo PUBLIC mo2::uibase PRIVATE ZLIB::ZLIB lz4::lz4 Dbghelp)
target_include_directories(game_gamebryo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(FILES $<TARGET_FILE:game_gamebryo> DESTINATION lib)

