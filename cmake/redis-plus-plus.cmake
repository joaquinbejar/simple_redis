find_path(REDISCPP_HEADER
        NAMES sw/redis++/redis++.h
        PATHS /usr/local/include
        REQUIRED
        )

find_library(REDISCPP_LIB
        NAMES redis++
        HINTS /usr/local/lib/
        REQUIRED
        )

if (REDISCPP_HEADER AND REDISCPP_LIB)
    message(STATUS "ROOT/REDIS REDISCPP FOUND")
    message(STATUS "ROOT/REDIS FOUND REDISCPP_HEADER ${REDISCPP_HEADER}")
    message(STATUS "ROOT/REDIS FOUND REDISCPP_LIB ${REDISCPP_LIB}")
endif ()