CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

CMAKE_POLICY(SET CMP0020 NEW)

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

SET(default_project_path "${CMAKE_SOURCE_DIR}/..")
GET_FILENAME_COMPONENT(${default_project_path} ${default_project_path} REALPATH)

SET(project_path "${default_project_path}" CACHE PATH "path to the other mo projects")
SET(lib_path "${project_path}/../../install/libs")
SET(plugin_path "${project_path}")

MESSAGE(STATUS ${lib_path})
MESSAGE(STATUS ${plugin_path})
MESSAGE(STATUS ${project_path})

INCLUDE_DIRECTORIES(${project_path}/uibase/src
					${project_path}/game_features/src)
LINK_DIRECTORIES(${lib_path})


ADD_LIBRARY(${PROJ_NAME} STATIC ${${PROJ_NAME}_HDRS} ${${PROJ_NAME}_SRCS} ${${PROJ_NAME}_UIHDRS})
TARGET_LINK_LIBRARIES(${PROJ_NAME}
                      Qt5::Widgets
                      ${Boost_LIBRARIES}
                      uibase)

SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES COMPILE_FLAGS /GL)
SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/LTCG /LARGEADDRESSAWARE /OPT:REF /OPT:ICF")

QT5_USE_MODULES(${PROJ_NAME} Widgets)

###############
## Installation

INSTALL(TARGETS ${PROJ_NAME}
        ARCHIVE DESTINATION libs)
