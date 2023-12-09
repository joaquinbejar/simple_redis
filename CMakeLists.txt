cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(simple_redis LANGUAGES CXX)


set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
set(REDIS_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_definitions(NDEBUG)
endif()

include(cmake/hiredis.cmake)
include(cmake/redis-plus-plus.cmake)
include(cmake/simple_logger.cmake)

set(SIMPLE_REDIS_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SIMPLE_REDIS_SOURCE_FILES
        include/simple_redis/config.h
        include/simple_redis/basic.h
        include/simple_redis/client.h
        include/simple_redis/fifo.h
        include/simple_redis/exceptions.h
        include/simple_redis/hash.h
        include/simple_redis/lifo.h
        include/simple_redis/set.h
        src/config.cpp
        src/client.cpp
        src/exceptions.cpp
        src/basic.cpp
        src/fifo.cpp
        src/hash.cpp
        src/lifo.cpp
        src/set.cpp
)

add_library(simple_redis STATIC ${SIMPLE_REDIS_SOURCE_FILES})
set(SIMPLE_REDIS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SIMPLE_REDIS_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)


if (CMAKE_DEBUG)
        message(STATUS "simple_redis HIREDIS_HEADER ${HIREDIS_HEADER}")
        message(STATUS "simple_redis HIREDIS_LIB ${HIREDIS_LIB}")
        message(STATUS "simple_redis REDISCPP_HEADER ${REDISCPP_HEADER}")
        message(STATUS "simple_redis REDISCPP_LIB ${REDISCPP_LIB}")
        message(STATUS "simple_redis SIMPLE_COLOR_INCLUDE: ${SIMPLE_COLOR_INCLUDE}")
        message(STATUS "simple_redis SIMPLE_CONFIG_INCLUDE: ${SIMPLE_CONFIG_INCLUDE}")
        message(STATUS "simple_redis SIMPLE_LOGGER_INCLUDE: ${SIMPLE_LOGGER_INCLUDE}")
        message(STATUS "simple_redis NLOHMANN_JSON_INCLUDE: ${NLOHMANN_JSON_INCLUDE}")
        message(STATUS "simple_redis COMMON_INCLUDE: ${COMMON_INCLUDE}")
        message(STATUS "simple_redis SIMPLE_REDIS_INCLUDE: ${SIMPLE_REDIS_INCLUDE}")
endif ()

target_include_directories(simple_redis PUBLIC
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
#        ${REDISCPP_HEADER}
#        ${REDISCPP_HEADER17}
#        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}
)

target_link_libraries(simple_redis
        PUBLIC
        pthread
        simple_config
        simple_logger
        common
#        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
)





set(SIMPLE_REDIS_TESTS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
option(NO_SIMPLE_REDIS_TESTS "simple_redis Disable simple_redis tests" OFF)
if (NOT NO_SIMPLE_REDIS_TESTS)
        set(CURL_DISABLE_TESTS ON)
        add_subdirectory(tests)
endif ()



