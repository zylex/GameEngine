#cmake_minimum_required (VERSION 3.1.2)
cmake_minimum_required (VERSION 2.8.8)
project (gameengine)

#message(STATUS "Building: ${PROJECT_NAME}")

# Use c++ 11
SET(CMAKE_CXX_FLAGS "-std=c++11" CACHE INTERNAL "Set C++11 on" FORCE)
SET(OPEN_GL FALSE CACHE BOOL "Use OpenGL")
SET(DIRECT_X FALSE CACHE BOOL "Use DirectX")
SET(USE_ANT FALSE CACHE BOOL "Use AntTweakBar")
SET(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (Debug, Release)")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

file(GLOB_RECURSE SRCS_ALL "src/*.cpp")
file(GLOB_RECURSE GAME_ENGINE_HEADERS "src/*.h")

list(APPEND SRCS_ALL ${GAME_ENGINE_HEADERS})

#SET(GAME_ENGINE_INCLUDE_DIRS "")
FOREACH(headerFile ${GAME_ENGINE_HEADERS})
    get_filename_component(dir ${headerFile} PATH)
    list(APPEND GAME_ENGINE_INCLUDE_DIRS ${dir})
ENDFOREACH()
list(REMOVE_DUPLICATES GAME_ENGINE_INCLUDE_DIRS)

find_package(GLM REQUIRED)
IF(NOT GLM_FOUND)
    message(ERROR "GLM not found")
ENDIF()
list(APPEND GAME_ENGINE_INCLUDE_DIRS ${GLM_INCLUDE_DIRS})
list(APPEND GAME_ENGINE_LIBRARY_PATHS ${GLM_LIBRARIES})

IF(CMAKE_BUILD_TYPE MATCHES Debug OR USE_ANT)
    message(STATUS "Using AntTweakBar")
    add_definitions(-DUSE_ANT)
    # TODO: find ant
ENDIF()

include(DetectSystem)

IF(DIRECT_X)
    include(ConfigureDirectXLibraries)
    include(ConfigureHLSLHeaders)
ELSEIF(OPEN_GL)
    include(ConfigureOpenGLLibraries)
    include(ConfigureGLSLHeaders)
ENDIF()

include(RemoveUnusedSrcs)

include_directories(${GAME_ENGINE_INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${GAME_ENGINE_SOURCES})

target_link_libraries(${PROJECT_NAME} ${GAME_ENGINE_LIBRARY_PATHS})

target_include_directories (${PROJECT_NAME} PUBLIC ${GAME_ENGINE_INCLUDE_DIRS})

#include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/DetectDistro.cmake")

IF (NOT TEST_DISABLED AND NOT CMAKE_BUILD_TYPE MATCHES Release)
    add_subdirectory(test)
ENDIF()

#message(STATUS "Finished building: ${PROJECT_NAME}")