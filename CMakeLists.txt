cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(simple_redis_base LANGUAGES CXX)

set(GITUSER "$ENV{GITUSER}")
set(GITPASS "$ENV{GITPASS}")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
set(CMAKE_BUILD_TYPE Debug)
set(REDIS_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "ROOT/REDIS REDIS_PROJECT_DIR: ${REDIS_PROJECT_DIR}")


find_path(JSON_INCLUDE_DIR
        NAMES nlohmann/json.hpp
        PATHS /usr/local/include
        REQUIRED
        )

include(cmake/hiredis.cmake)
include(cmake/redis-plus-plus.cmake)
include(cmake/simple_common_base.cmake)


set(SIMPLE_POLYGON_IO_TESTS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
option(NO_POLYGON_IO_TESTS "simple_polygon_io Disable simple_polygon_io tests" OFF)
if (NOT NO_POLYGON_IO_TESTS)
        #    set -DCURL_DISABLE_TESTS=ON to disable curl tests
        set(CURL_DISABLE_TESTS ON)
        add_subdirectory(tests)
endif ()



