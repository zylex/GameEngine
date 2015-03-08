IF(DIRECT_X)
    IF(CMAKE_BUILD_TYPE MATCHES Release)
        message(STATUS "Release build detected.")
        message(STATUS "Don't know how to configure shader headers for this platform: ${CMAKE_SYSTEM_NAME}.")
        # TODO: Implement using the microsoft compiler
        # find fxc.exe and use the docs to set correct env vars
    ELSE()
        # debug has nothing to do but define DEBUG for compiler
        # create files taht compile the files to byte code
        message(STATUS "Configuring shader headers.")
        message(STATUS "Don't know how to configure shader headers for this platform: ${CMAKE_SYSTEM_NAME}.")
        # TODO: Implement
        list(APPEND SRCS_ALL ${CMAKE_CURRENT_SOURCE_DIR}/debug_includes/FileHelper.cpp)
        list(APPEND SRCS_ALL ${CMAKE_CURRENT_SOURCE_DIR}/debug_includes/FileHelper.h)
        list(APPEND GAME_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/debug_includes)
    ENDIF()
ENDIF()