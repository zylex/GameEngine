IF(DIRECT_X)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)
    IF(CMAKE_BUILD_TYPE MATCHES Release)
        message(STATUS "Release build detected.")
        message(STATUS "Searching for fxc.exe...")
        find_program( FXC
            NAMES
                fxc.exe
            PATHS
                "$ENV{PROGRAMFILES}/Windows Kits/8.0/bin/x86"
                "$ENV{PROGRAMFILES}/Windows Kits/8.0/bin/x64"
                "$ENV{PROGRAMFILES}/Windows Kits/8.1/bin/x86"
                "$ENV{PROGRAMFILES}/Windows Kits/8.1/bin/x64"
            DOC
                "The directory where the fxc.exe HLSL compiler resides"
        )

        IF(NOT FXC)
            message(FATAL_ERROR "Could not find fxc.exe")
        ELSE()
            message(STATUS "Found fxc.exe with path: ${FXC}")
        ENDIF()

        SET(DEBUG_DX11_SHADER FALSE CACHE BOOL "Add Debug Symbols to the compiler")

        message(STATUS "Creating HLSL shader header targets.")
        file(GLOB_RECURSE shader_list src/*.hlsl)
        FOREACH(file_path ${shader_list})
            get_filename_component(file_name ${file_path} NAME_WE)
            list(APPEND SHADER_HEADERS ${CMAKE_BINARY_DIR}/shaders/${file_name}.h)
            string(REGEX MATCH "VS$" shader_type ${file_name})
            IF(NOT shader_type)
                string(REGEX MATCH "PS$" shader_type ${file_name})
                IF(NOT shader_type)
                    string(REGEX MATCH "CS$" shader_type ${file_name})
                    IF(NOT shader_type)
                        string(REGEX MATCH "GS$" shader_type ${file_name})
                        IF(NOT shader_type)
                            message(STATUS "File path: ${file_path}")
                            message(FATAL_ERROR "Could not determine shader type: ${file_name}")
                        ENDIF()
                    ENDIF()
                ENDIF()
            ENDIF()
            string(TOLOWER "${shader_type}" shader_type)
            SET(size_line_of_code const std::size_t ${file_name}_size "=" "sizeof(${file_name})$<SEMICOLON>")

            IF(CMAKE_DEBUG_OUTPUT)
                message(STATUS "Filepath: ${file_path}")
                message(STATUS "out: ${CMAKE_BINARY_DIR}/${file_name}.h}")
            ENDIF()

            STRING(REPLACE "/" "\\" WIN_BIN_DIR ${CMAKE_BINARY_DIR})

            IF(DEBUG_DX11_SHADER)
                add_custom_command(
                    OUTPUT ${CMAKE_BINARY_DIR}/shaders/${file_name}.h
                    COMMAND ${FXC} /O0 /E main /Vn ${file_name} /T ${shader_type}_5_0 /nologo /Zi /Fc ${WIN_BIN_DIR}\\shaders\\${file_name}.cod /Fh ${WIN_BIN_DIR}\\shaders\\${file_name}.h.tmp ${file_path}# /Zpc
                    COMMAND echo ${size_line_of_code} >> ${WIN_BIN_DIR}\\shaders\\${file_name}.h.tmp
                    COMMAND echo \#include ^<windows.h^> > ${WIN_BIN_DIR}\\shaders\\${file_name}.h
                    COMMAND type ${WIN_BIN_DIR}\\shaders\\${file_name}.h.tmp >> ${WIN_BIN_DIR}\\shaders\\${file_name}.h
                    COMMAND del ${WIN_BIN_DIR}\\shaders\\${file_name}.h.tmp
                    MAIN_DEPENDENCY ${file_path}
                    DEPENDS ${file_path}
                    COMMENT "Compiling ${file_name}"
                )
            ELSE()
                add_custom_command(
                    OUTPUT ${CMAKE_BINARY_DIR}/shaders/${file_name}.h
                    COMMAND ${FXC} /O3 /Qstrip_debug /Qstrip_priv /E main /Vn ${file_name} /Fh ${CMAKE_BINARY_DIR}/shaders/${file_name}.h /T ${shader_type}_5_0 /nologo ${file_path}# /Zpc
                    COMMAND echo ${size_line_of_code} >> ${WIN_BIN_DIR}\\shaders\\${file_name}.h.tmp
                    COMMAND echo \#include ^<windows.h^> > ${WIN_BIN_DIR}\\shaders\\${file_name}.h
                    COMMAND type ${WIN_BIN_DIR}\\shaders\\${file_name}.h.tmp >> ${WIN_BIN_DIR}\\shaders\\${file_name}.h
                    COMMAND del ${WIN_BIN_DIR}\\shaders\\${file_name}.h.tmp
                    MAIN_DEPENDENCY ${file_path}
                    DEPENDS ${file_path}
                    COMMENT "Compiling ${file_name}"
                )
            ENDIF()
            set_source_files_properties(${file_name}.h PROPERTIES GENERATED TRUE)
            set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${file_name}.h)
        ENDFOREACH()

        message(STATUS "SHADER_HEADERS: ${SHADER_HEADERS}")

        add_custom_target(
            PreCompileShaders
            DEPENDS ${SHADER_HEADERS}
            COMMENT "Pre-compiling Shader files."
        )

        list(APPEND GAME_ENGINE_DEPENDENCIES PreCompileShaders)
    ELSE()
        message(STATUS "Configuring shader headers.")
        file(GLOB_RECURSE shader_list src/*.hlsl)
        FOREACH(file_path ${shader_list})
            get_filename_component(file_name ${file_path} NAME_WE)
            message(STATUS "-- Configuring header for ${file_name}.hlsl.")
            configure_file (
                "${CMAKE_CURRENT_SOURCE_DIR}/cmake/HLSLShaderHeader.h.in"
                "${CMAKE_BINARY_DIR}/shaders/${file_name}.h"
            )
        ENDFOREACH()
    ENDIF()
    list(APPEND GAME_ENGINE_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/shaders)
ENDIF()
