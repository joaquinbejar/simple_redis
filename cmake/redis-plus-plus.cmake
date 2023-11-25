#find_path(REDISCPP_HEADER
#        NAMES sw/redis++/redis++.h
#        PATHS /usr/local/include
#        REQUIRED
#        )
#
#find_library(REDISCPP_LIB
#        NAMES redis++
#        HINTS /usr/local/lib/
#        REQUIRED
#        )
#
#if (REDISCPP_HEADER AND REDISCPP_LIB)
#    message(STATUS "ROOT/REDIS REDISCPP FOUND")
#    message(STATUS "ROOT/REDIS FOUND REDISCPP_HEADER ${REDISCPP_HEADER}")
#    message(STATUS "ROOT/REDIS FOUND REDISCPP_LIB ${REDISCPP_LIB}")
#endif ()


include(FetchContent)
FetchContent_Declare(rediscpp
        GIT_REPOSITORY https://github.com/sewenew/redis-plus-plus.git
        GIT_TAG 1.3.10
        CMAKE_ARGS -DREDIS_PLUS_PLUS_USE_TLS=OFF -DREDIS_PLUS_PLUS_CXX_STANDARD=17 -DREDIS_PLUS_PLUS_BUILD_TEST=OFF -DREDIS_PLUS_PLUS_BUILD_STATIC=ON -DREDIS_PLUS_PLUS_BUILD_SHARED=OFF
        BUILD_ALWAYS 1
)
FetchContent_MakeAvailable(rediscpp)

set(REDISCPP_INCLUDE "${rediscpp_SOURCE_DIR}/src" CACHE INTERNAL "")
set(REDISCPP_HEADER "${rediscpp_SOURCE_DIR}/src" CACHE INTERNAL "")
set(REDISCPP_HEADER17 "${rediscpp_SOURCE_DIR}/src/sw/redis++/cxx17" CACHE INTERNAL "")
set(REDISCPP_HEADERTLS "${rediscpp_SOURCE_DIR}/src/sw/redis++/no_tls" CACHE INTERNAL "")
set(REDISCPP_LIB "${rediscpp_BINARY_DIR}/libredis++.a" CACHE INTERNAL "")
set(TEST_REDISCPP_LIB "${rediscpp_BINARY_DIR}/libredis++.a" CACHE INTERNAL "")

if (CMAKE_DEBUG)
    message(STATUS "simple_mariadb/cmake rediscpp_SOURCE_DIR ${rediscpp_SOURCE_DIR}")
    message(STATUS "simple_mariadb/cmake REDISCPP_INCLUDE ${REDISCPP_INCLUDE}")
endif ()