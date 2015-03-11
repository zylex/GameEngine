IF(OPEN_GL)
    IF(CMAKE_BUILD_TYPE MATCHES Release)
        message(STATUS "Release build detected.")
        message(STATUS "Creating GLSL shader header targets.")
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)
        SET(tmp ${CMAKE_BINARY_DIR})
        SET(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/shaders)
        add_executable(embedfile ${CMAKE_CURRENT_SOURCE_DIR}/cmake/embedfile.cpp)
        SET(CMAKE_BINARY_DIR ${tmp})
        file(GLOB_RECURSE shader_list src/*.glsl)
        FOREACH(file_path ${shader_list})
            get_filename_component(file_name ${file_path} NAME_WE)
            list(APPEND SHADER_HEADERS ${CMAKE_BINARY_DIR}/shaders/${file_name}.h)
            add_custom_command(
                OUTPUT ${CMAKE_BINARY_DIR}/shaders/${file_name}.h
                COMMAND embedfile ${file_name} ${file_path} ${CMAKE_BINARY_DIR}/${file_name}.h
                DEPENDS ${file_path}
                IMPLICIT_DEPENDS embedfile
            )
            set_source_files_properties(${file_name}.h PROPERTIES GENERATED TRUE)
            set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${file_name}.h)
        ENDFOREACH()

        add_custom_target(
            PreCompileShaders
            DEPENDS ${SHADER_HEADERS}
        )

        list(APPEND GAME_ENGINE_DEPENDENCIES PreCompileShaders)

        list(APPEND GAME_ENGINE_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/shaders)
    ELSE()
        message(STATUS "Configuring shader headers.")
        file(GLOB_RECURSE shader_list src/*.glsl)
        FOREACH(file_path ${shader_list})
            get_filename_component(file_name ${file_path} NAME_WE)
            message(STATUS "-- Configuring header for ${file_name}.glsl.")
            configure_file (
                "${CMAKE_CURRENT_SOURCE_DIR}/cmake/GLSLShaderHeader.h.in"
                "${CMAKE_CURRENT_SOURCE_DIR}/debug_includes/${file_name}.h"
            )
        ENDFOREACH()
        list(APPEND SRCS_ALL ${CMAKE_CURRENT_SOURCE_DIR}/debug_includes/FileHelper.cpp)
        list(APPEND SRCS_ALL ${CMAKE_CURRENT_SOURCE_DIR}/debug_includes/FileHelper.h)
        list(APPEND GAME_ENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/debug_includes)
    ENDIF()
ENDIF()