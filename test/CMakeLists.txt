cmake_minimum_required (VERSION 3.1.2)
project (gameengine_test)

SET(CMAKE_CXX_FLAGS "-std=c++11")

enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

file(GLOB_RECURSE TEST_SRCS "src/*.cpp")


include(GetCatch)
list(APPEND TEST_INCLUDE_DIRS ${CATCH_INCLUDE_DIR})

include_directories(${TEST_INCLUDE_DIRS})

FOREACH(test ${TEST_SRCS})
get_filename_component(test ${test} NAME_WE)
    add_test(NAME ${test} COMMAND ${PROJECT_NAME} ${test})
ENDFOREACH()

list(APPEND TEST_SRCS "main.cpp")

add_executable(${PROJECT_NAME} ${TEST_SRCS})

target_link_libraries(${PROJECT_NAME} ${TEST_LIBRARY_PATHS} gameengine)
