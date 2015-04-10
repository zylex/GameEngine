#cmake_minimum_required (VERSION 3.1.2)
cmake_minimum_required (VERSION 2.8.8)
project (gameengine)

SET(CMAKE_CXX_FLAGS "")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

SET(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (Debug, Release)")
SET(TEST_DISABLED TRUE CACHE BOOL "Include Tests")
SET(REMOVE_SRCS FALSE CACHE BOOL "Remove unsued source files (Direct X or OpenGL)")
SET(USE_ANT FALSE CACHE BOOL "Use AntTweakBar")

SET(GAME_ENGINE_DEPENDENCIES "")

include(DetectSystem)

set(SRCS_ALL "")
list(APPEND SRCS_ALL "")
set(GAME_ENGINE_HEADERS "")
list(APPEND GAME_ENGINE_HEADERS "")
file(GLOB_RECURSE SRCS_ALL "src/*.cpp")
file(GLOB_RECURSE GAME_ENGINE_HEADERS "src/*.h")

list(APPEND SRCS_ALL ${GAME_ENGINE_HEADERS})

FOREACH(headerFile ${GAME_ENGINE_HEADERS})
    get_filename_component(dir ${headerFile} PATH)
    list(APPEND GAME_ENGINE_INCLUDE_DIRS ${dir})
ENDFOREACH()
list(REMOVE_DUPLICATES GAME_ENGINE_INCLUDE_DIRS)

find_package(GLM REQUIRED)
IF(NOT GLM_FOUND)
    message(FATAL_ERROR "GLM not found")
ENDIF()
list(APPEND GAME_ENGINE_INCLUDE_DIRS ${GLM_INCLUDE_DIRS})
list(APPEND GAME_ENGINE_LIBRARY_PATHS ${GLM_LIBRARIES})

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
    SET(USE_ANT TRUE CACHE BOOL "Use AntTweakBar")
ELSE()
    SET(USE_ANT FALSE CACHE BOOL "Use AntTweakBar")
ENDIF()

IF(USE_ANT)
    message(STATUS "Using AntTweakBar")
    add_definitions(-DUSE_ANT)
    find_package(AntTweakBar REQUIRED)
    IF(NOT TW_FOUND)
        message(FATAL_ERROR "AntTweakBar not found")
    ENDIF()
    list(APPEND GAME_ENGINE_INCLUDE_DIRS ${TW_INCLUDE_DIRS})
    list(APPEND GAME_ENGINE_LIBRARY_PATHS ${TW_LIBRARIES})
ENDIF()

IF(DIRECT_X)
    include(ConfigureDirectXLibraries)
    include(ConfigureHLSLHeaders)
ELSEIF(OPEN_GL)
    # Use c++ 11
    SET(CMAKE_CXX_FLAGS "-std=c++11" CACHE INTERNAL "Set C++11 on" FORCE)
    include(ConfigureOpenGLLibraries)
    include(ConfigureGLSLHeaders)
ENDIF()

include(RemoveUnusedSrcs)

include_directories(${GAME_ENGINE_INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${GAME_ENGINE_SOURCES})

target_link_libraries(${PROJECT_NAME} ${GAME_ENGINE_LIBRARY_PATHS})

target_include_directories (${PROJECT_NAME} PUBLIC ${GAME_ENGINE_INCLUDE_DIRS})

IF(GAME_ENGINE_DEPENDENCIES)
    IF(CMAKE_DEBUG_OUTPUT)
        message(STATUS "deps: ${GAME_ENGINE_DEPENDENCIES}")
    ENDIF()
    add_dependencies(${PROJECT_NAME} ${GAME_ENGINE_DEPENDENCIES})
ENDIF()

IF(NOT TEST_DISABLED AND NOT CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Adding tests subdirectory.")
    add_subdirectory(test)
ENDIF()

SET(CMAKE_DEBUG_OUTPUT FALSE CACHE BOOL "Whether or not to show debug output for CMake.")

IF(CMAKE_DEBUG_OUTPUT)
    get_directory_property(DirDefs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPILE_DEFINITIONS)
    message(STATUS "Compile Defs: ${DirDefs}")
ENDIF()
