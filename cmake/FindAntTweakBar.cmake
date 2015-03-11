FIND_PATH( TW_INCLUDE_DIR 
    NAMES
        AntTweakBar.h
    PATHS
        "/usr/include"
        "/usr/include/GL"
        "/usr/local/include"
        "$ENV{PROGRAMFILES}/AntTweakBar/include"
        "${OPENGL_INCLUDE_DIR}"
    DOC 
        "The directory where AntTweakBar.h resides"
)

FIND_LIBRARY( TW_LIBRARY
    NAMES 
        AntTweakBar
        AntTweakBar64
    PATHS
        /usr/lib64
        /usr/lib
        /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}
        /usr/local/lib64
        /usr/local/lib
        /usr/local/lib/${CMAKE_LIBRARY_ARCHITECTURE}
        "$ENV{PROGRAMFILES}/AntTweakBar/lib"
        "${OPENGL_LIBRARY_DIR}"
    DOC 
        "The AntTweakBar library"
)

#IF (TW_LIBRARY AND TW_INCLUDE_DIR)
#    SET(TW_FOUND TRUE)
#ENDIF()

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(TW 
    REQUIRED_VARS
        TW_INCLUDE_DIR
        TW_LIBRARIES
)