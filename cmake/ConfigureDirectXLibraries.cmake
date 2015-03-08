IF(DIRECT_X)
    add_definitions(-DDIRECT_X)

    # maybe compile the AntTweakBar dll

    find_package(DirectX11 REQUIRED)
    if(NOT DirectX11_FOUND)
            message(ERROR " DirectX11 not found")
    endif(NOT DirectX11_FOUND)
    list(APPEND GAME_ENGINE_INCLUDE_DIRS ${DirectX11_INCLUDE_DIRS})
    list(APPEND GAME_ENGINE_LIBRARY_PATHS ${DirectX11_LIBRARIES})
ENDIF()