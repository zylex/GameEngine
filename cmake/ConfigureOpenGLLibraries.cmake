IF(OPEN_GL)
    add_definitions(-DOPEN_GL)

    find_package(OpenGL REQUIRED)
    if(NOT OPENGL_FOUND)
        message(ERROR "OpenGL not found")
    endif(NOT OPENGL_FOUND)
    list(APPEND GAME_ENGINE_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
    list(APPEND GAME_ENGINE_LIBRARY_PATHS ${OPENGL_LIBRARIES})

    find_package(GLEW REQUIRED)
    if(NOT GLEW_FOUND)
        message(ERROR "GLEW not found")
    endif(NOT GLEW_FOUND)
    list(APPEND GAME_ENGINE_INCLUDE_DIRS ${GLEW_INCLUDE_DIRS})
    list(APPEND GAME_ENGINE_LIBRARY_PATHS ${GLEW_LIBRARIES})

    find_package(GLFW REQUIRED glfw3)
    if(NOT GLFW_FOUND)
        message(ERROR "GLFW not found")
    endif(NOT GLFW_FOUND)
    list(APPEND GAME_ENGINE_INCLUDE_DIRS ${GLFW_INCLUDE_DIRS})
    list(APPEND GAME_ENGINE_LIBRARY_PATHS ${GLFW_LIBRARIES})
ENDIF()