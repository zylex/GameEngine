cmake_minimum_required (VERSION 3.1.2)
project (gameengine)

SET(CMAKE_CXX_FLAGS "-std=c++11")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

#include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/HeaderDirs.cmake")

find_package(PkgConfig REQUIRED)

find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
    message(Error "GLEW not found")
endif(NOT GLEW_FOUND)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(GLFW REQUIRED glfw3)
if(NOT GLFW_FOUND)
    message(Error "GLFW not found")
endif(NOT GLFW_FOUND)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(GLM REQUIRED)
if(NOT GLM_FOUND)
        message(Error "GLM not found")
endif(NOT GLM_FOUND)
include_directories(${GLM_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
        message(Error "OpenGL not found")
endif(NOT OPENGL_FOUND)
include_directories(${OPENGL_INCLUDE_DIR})

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "*.cpp")
file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS "*.h")

set (${PROJECT_NAME}_INCLUDE_DIRS "")
foreach (headerFile ${${PROJECT_NAME}_HEADERS})
    get_filename_component(dir ${headerFile} PATH)
    list (APPEND ${PROJECT_NAME}_INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES ${PROJECT_NAME}_INCLUDE_DIRS)

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${GLM_LIBRARIES})

target_include_directories (${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_INCLUDE_DIRS})