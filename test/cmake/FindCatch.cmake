SET( CATCH_FOUND "NO" )

find_path( CATCH_INCLUDE_DIR 
    NAMES
        catch.hpp
    PATHS
        "${CATCH_LOCATION}/include"
        "$ENV{CATCH_LOCATION}/include"
        "$ENV{PROGRAMFILES}/catch/include"
        /usr/local/include
        /usr/include
    DOC 
        "The directory where catch.hpp resides"
)

IF(CATCH_INCLUDE_DIR)
    message(STATUS "Found Catch: ${CATCH_INCLUDE_DIR}")
    SET( CATCH_FOUND "YES" )
ENDIF()
