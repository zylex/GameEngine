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

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/*.cpp")
file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS "src/*.h")

set (${PROJECT_NAME}_INCLUDE_DIRS "")
foreach (headerFile ${${PROJECT_NAME}_HEADERS})
    get_filename_component(dir ${headerFile} PATH)
    list(APPEND ${PROJECT_NAME}_INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES ${PROJECT_NAME}_INCLUDE_DIRS)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Debug build detected.")
    message(STATUS "Configuring shader headers.")
    file(GLOB_RECURSE shader_list src/*.glsl)
    FOREACH(file_path ${shader_list})
        get_filename_component(file_name ${file_path} NAME_WE)
        message(STATUS "-- Configuring header for ${file_name}.glsl.")
        configure_file (
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ShaderHeader.h.in"
            "${CMAKE_CURRENT_SOURCE_DIR}/debug_includes/${file_name}.h"
        )
    ENDFOREACH()
    list(APPEND ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/debug_includes/FileHelper.cpp)
    #include_directories(${CMAKE_CURRENT_SOURCE_DIR}/debug_includes)
    list(APPEND ${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/debug_includes)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Release build detected.")
    message(STATUS "Creating GLSL shader header targets.")
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)
    SET(tmp ${CMAKE_BINARY_DIR})
    SET(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/shaders)
    add_executable(embedfile ${CMAKE_CURRENT_SOURCE_DIR}/cmake/embedfile.cpp)
    file(GLOB_RECURSE shader_list src/*.glsl)
    FOREACH(file_path ${shader_list})
        get_filename_component(file_name ${file_path} NAME_WE)
        list(APPEND ${PROJECT_NAME}_SOURCES ${CMAKE_BINARY_DIR}/${file_name}.h)
        add_custom_command(
            OUTPUT ${CMAKE_BINARY_DIR}/${file_name}.h
            COMMAND embedfile ${file_name} ${file_path} ${CMAKE_BINARY_DIR}/${file_name}.h
            DEPENDS ${file_path}
            IMPLICIT_DEPENDS embedfile
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    ENDFOREACH()
    SET(CMAKE_BINARY_DIR ${tmp})
    #include_directories(${CMAKE_BINARY_DIR}/shaders)
    list(APPEND ${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/shaders)
#ELSE()
#    message("Some other build type.")
ENDIF()

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${GLM_LIBRARIES})

target_include_directories (${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_INCLUDE_DIRS})

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/DetectDistro.cmake")