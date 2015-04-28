#cmake_minimum_required (VERSION 3.1.2)
cmake_minimum_required (VERSION 2.8.8)
project (gameengine_test)

#message(STATUS "Building: ${PROJECT_NAME}")

SET(CMAKE_CXX_FLAGS "-std=c++11")

enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

file(GLOB_RECURSE TEST_SRCS "src/*.cpp")

#list(APPEND TEST_SRCS "main.cpp")

#find_package(GTest REQUIRED)
#IF(NOT GTEST_FOUND)
#    message(ERROR "GTest not found")
#ENDIF(NOT GTEST_FOUND)
#list(APPEND TEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIRS})
#list(APPEND TEST_LIBRARY_PATHS ${GTEST_LIBRARIES})

#find_package(GMock REQUIRED)
#IF(NOT GMOCK_FOUND)
#    message(ERROR "GMock not found")
#ENDIF(NOT GMOCK_FOUND)
#list(APPEND TEST_INCLUDE_DIRS ${GMOCK_INCLUDE_DIRS})
#list(APPEND TEST_LIBRARY_PATHS ${GMOCK_LIBRARIES})

include(GetCatch)
list(APPEND TEST_INCLUDE_DIRS ${CATCH_INCLUDE_DIR})

include_directories(${TEST_INCLUDE_DIRS})

#add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

#GTEST_ADD_TESTS(${PROJECT_NAME} "" ${TEST_SRCS})
FOREACH(test ${TEST_SRCS})
get_filename_component(test ${test} NAME_WE)
    add_test(NAME ${test} COMMAND ${PROJECT_NAME} ${test})
ENDFOREACH()

#message(STATUS "Finished building: ${PROJECT_NAME}")
list(APPEND TEST_SRCS "main.cpp")

add_executable(${PROJECT_NAME} ${TEST_SRCS})

target_link_libraries(${PROJECT_NAME} ${TEST_LIBRARY_PATHS} gameengine)
