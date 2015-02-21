IF(DIRECT_X)
    add_definitions(-DDIRECT_X)

    # maybe compile the AntTweakBar dll

    find_package(DirectX11 REQUIRED)
    if(NOT DIRECT_X_FOUND)
            message(ERROR "DirectX not found")
    endif(NOT DIRECT_X_FOUND)
    list(APPEND GAME_ENGINE_INCLUDE_DIRS ${DIRECT_X_INCLUDE_DIR})
    list(APPEND GAME_ENGINE_LIBRARY_PATHS ${DIRECT_X_LIBRARIES})
ENDIF()