find_path(HIREDIS_HEADER
        NAMES hiredis/hiredis.h
        PATHS /usr/local/include
        REQUIRED
        )
find_library(HIREDIS_LIB
        NAMES hiredis
        HINTS /usr/local/lib/
        REQUIRED
        )
if (HIREDIS_HEADER AND HIREDIS_LIB)
    message(STATUS "ROOT/REDIS HIREDIS FOUND")
    message(STATUS "ROOT/REDIS FOUND HIREDIS_HEADER ${HIREDIS_HEADER}")
    message(STATUS "ROOT/REDIS FOUND HIREDIS_LIB ${HIREDIS_LIB}")
endif ()
