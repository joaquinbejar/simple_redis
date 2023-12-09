include(FetchContent)
FetchContent_Declare(rediscpp
        GIT_REPOSITORY https://github.com/sewenew/redis-plus-plus.git
        GIT_TAG 1.3.10
#        CMAKE_ARGS -DREDIS_PLUS_PLUS_USE_TLS=OFF -DREDIS_PLUS_PLUS_CXX_STANDARD=17 -DREDIS_PLUS_PLUS_BUILD_TEST=OFF -DREDIS_PLUS_PLUS_BUILD_STATIC=ON -DREDIS_PLUS_PLUS_BUILD_SHARED=OFF
        BUILD_ALWAYS 1
)
#FetchContent_MakeAvailable(rediscpp)
#
#set(REDISCPP_INCLUDE "${rediscpp_SOURCE_DIR}/src" CACHE INTERNAL "")
#set(REDISCPP_HEADER "${rediscpp_SOURCE_DIR}/src" CACHE INTERNAL "")
#set(REDISCPP_HEADER17 "${rediscpp_SOURCE_DIR}/src/sw/redis++/cxx17" CACHE INTERNAL "")
#set(REDISCPP_HEADERTLS "${rediscpp_SOURCE_DIR}/src/sw/redis++/no_tls" CACHE INTERNAL "")
#set(REDISCPP_LIB "${rediscpp_BINARY_DIR}/libredis++.a" CACHE INTERNAL "")
#set(TEST_REDISCPP_LIB "${rediscpp_BINARY_DIR}/libredis++.a" CACHE INTERNAL "")
#
#if (CMAKE_DEBUG)
#    message(STATUS "simple_redis/cmake rediscpp_SOURCE_DIR ${rediscpp_SOURCE_DIR}")
#    message(STATUS "simple_redis/cmake REDISCPP_INCLUDE ${REDISCPP_INCLUDE}")
#endif ()


FetchContent_Populate(rediscpp)

if(NOT EXISTS "${REDISCPP_LIB}")
    message(STATUS "rediscpp library not found, building it")
    execute_process(
            COMMAND ${CMAKE_COMMAND} -DCMAKE_PREFIX_PATH=${HIREDIS_HEADER} -DCMAKE_MODULE_PATH=${HIREDIS_HEADER} -DREDIS_PLUS_PLUS_USE_TLS=OFF -DREDIS_PLUS_PLUS_CXX_STANDARD=17 -DREDIS_PLUS_PLUS_BUILD_TEST=OFF -DREDIS_PLUS_PLUS_BUILD_STATIC=ON -DREDIS_PLUS_PLUS_BUILD_SHARED=OFF -S ${rediscpp_SOURCE_DIR} -B ${rediscpp_BINARY_DIR}
            WORKING_DIRECTORY ${rediscpp_SOURCE_DIR}
            COMMAND_ECHO STDOUT
            COMMAND_ECHO STDERR
            RESULT_VARIABLE result
    )
    if (result)
        message(FATAL_ERROR "Error Configuring rediscpp ${result}")
    endif ()

    execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${rediscpp_BINARY_DIR}
            WORKING_DIRECTORY ${rediscpp_SOURCE_DIR}
            COMMAND_ECHO STDOUT
            COMMAND_ECHO STDERR
            RESULT_VARIABLE result
    )
    if (result)
        message(FATAL_ERROR "Error building rediscpp ${result}")
    endif ()

else()
    message(STATUS "rediscpp library found")
endif()

set(rediscpp_HEADER ${rediscpp_SOURCE_DIR}/include CACHE INTERNAL "")
find_library(REDISCPP_LIB
        NAMES redis++
        PATHS ${rediscpp_BINARY_DIR}
        REQUIRED
)


set(REDISCPP_INCLUDE "${rediscpp_SOURCE_DIR}/src" CACHE INTERNAL "")
set(REDISCPP_HEADER "${rediscpp_SOURCE_DIR}/src" CACHE INTERNAL "")
set(REDISCPP_HEADER17 "${rediscpp_SOURCE_DIR}/src/sw/redis++/cxx17" CACHE INTERNAL "")
set(REDISCPP_HEADERTLS "${rediscpp_SOURCE_DIR}/src/sw/redis++/no_tls" CACHE INTERNAL "")

if (CMAKE_DEBUG)
    message(STATUS "simple_redis/cmake rediscpp_SOURCE_DIR ${rediscpp_SOURCE_DIR}")
    message(STATUS "simple_redis/cmake REDISCPP_LIB ${REDISCPP_LIB}")
    message(STATUS "simple_redis/cmake REDISCPP_INCLUDE ${REDISCPP_INCLUDE}")
endif ()