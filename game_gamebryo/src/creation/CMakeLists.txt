CMAKE_MINIMUM_REQUIRED (VERSION 2.8.11)

CMAKE_POLICY(SET CMP0020 NEW)

SET(PROJ_NAME game_creation)
PROJECT(${PROJ_NAME})

FILE(GLOB ${PROJ_NAME}_SRCS *.cpp)
FILE(GLOB ${PROJ_NAME}_HDRS *.h)
FILE(GLOB ${PROJ_NAME}_FORMS *.ui)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
FIND_PACKAGE(Qt5Widgets REQUIRED)
QT5_WRAP_UI(${PROJ_NAME}_UIHDRS ${${PROJ_NAME}_FORMS})

SET(Boost_USE_STATIC_LIBS        ON)
SET(Boost_USE_MULTITHREADED      ON)
SET(Boost_USE_STATIC_RUNTIME    OFF)
FIND_PACKAGE(Boost)

IF (Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
ENDIF (Boost_FOUND)

SET(default_project_path "${DEPENDENCIES_DIR}/modorganizer_super")
GET_FILENAME_COMPONENT(${default_project_path} ${default_project_path} REALPATH)

SET(project_path "${default_project_path}" CACHE PATH "path to the other mo projects")
SET(lib_path "${project_path}/../../install/libs")
SET(plugin_path "${project_path}")


INCLUDE_DIRECTORIES(../gamebryo
                    ${project_path}/uibase/src
                    ${project_path}/game_features/src
                    ${LZ4_ROOT}/lib)
LINK_DIRECTORIES(${lib_path}
                 ${LZ4_ROOT}/bin)

ADD_LIBRARY(${PROJ_NAME} STATIC ${${PROJ_NAME}_HDRS} ${${PROJ_NAME}_SRCS} ${${PROJ_NAME}_UIHDRS})
TARGET_LINK_LIBRARIES(${PROJ_NAME}
                      Qt5::Widgets
                      ${Boost_LIBRARIES}
                      uibase
                      liblz4
                      Version
                      game_gamebryo)

IF (MSVC)
  SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES COMPILE_FLAGS "/std:c++latest")
ENDIF()
IF (MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
  # 32 bits
  SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES LINK_FLAGS "/LARGEADDRESSAWARE")
ENDIF()

IF (NOT "${OPTIMIZE_COMPILE_FLAGS}" STREQUAL "")
  SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES COMPILE_FLAGS_RELWITHDEBINFO ${OPTIMIZE_COMPILE_FLAGS})
ENDIF()
IF (NOT "${OPTIMIZE_LINK_FLAGS}" STREQUAL "")
  SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO ${OPTIMIZE_LINK_FLAGS})
ENDIF()

###############
## Installation

INSTALL(TARGETS ${PROJ_NAME}
        ARCHIVE DESTINATION libs)
