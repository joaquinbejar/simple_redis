message(STATUS "TEST SIMPLE_COMMON_SOURCE_DIR ${simple_common_base_SOURCE_DIR}")
include(cmake/catch2.cmake)

message(STATUS "SIMPLE/REDIS/TEST SIMPLE_SOURCE_DIR ${SIMPLE_SOURCE_DIR}")
message(STATUS "SIMPLE/REDIS/TEST catch2_BINARY_DIR ${catch2_BINARY_DIR}")
message(STATUS "SIMPLE/REDIS/TEST REDIS_PROJECT_DIR: ${REDIS_PROJECT_DIR}")
message(STATUS "SIMPLE/REDIS/TEST FOUND REDISCPP_HEADER ${REDISCPP_HEADER}")
message(STATUS "SIMPLE/REDIS/TEST FOUND REDISCPP_LIB ${REDISCPP_LIB}")

add_executable(redisconfig_test redisconfig_test.cpp)
target_include_directories(redisconfig_test
        PRIVATE
        .
        ${catch2_SOURCE_DIR}
        ${REDISCPP_HEADER}
        ${HIREDIS_HEADER}
        ${REDIS_PROJECT_DIR}
        ${simple_common_base_SOURCE_DIR}
        ${JSON_INCLUDE_DIR}
        )
target_link_libraries(redisconfig_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(redisconfig_test
        PRIVATE
        pthread
        simple_color
        simple_config
        simple_logger
        simple_redis
        simple_common
        )


add_executable(redisclient_test redisclient_test.cpp)
target_include_directories(redisclient_test
        PRIVATE
        .
        ${catch2_SOURCE_DIR}
        ${REDISCPP_HEADER}
        ${HIREDIS_HEADER}
        ${REDIS_PROJECT_DIR}
        ${simple_common_base_SOURCE_DIR}
        ${JSON_INCLUDE_DIR}
        )
target_link_libraries(redisclient_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(redisclient_test
        PRIVATE
        pthread
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
        simple_color
        simple_config
        simple_logger
        simple_redis
        simple_common
        )


add_executable(redisfuncs_test redisfuncs_test.cpp)
target_include_directories(redisfuncs_test
        PRIVATE
        .
        ${catch2_SOURCE_DIR}
        ${REDISCPP_HEADER}
        ${HIREDIS_HEADER}
        ${REDIS_PROJECT_DIR}
        ${simple_common_base_SOURCE_DIR}
        ${JSON_INCLUDE_DIR}
        )
target_link_libraries(redisfuncs_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(redisfuncs_test
        PRIVATE
        pthread
        simple_color
        simple_config
        simple_logger
        simple_redis
        simple_common
        )

add_executable(hashredisclient_test hashredisclient_test.cpp)
target_include_directories(hashredisclient_test
        PRIVATE
        .
        ${catch2_SOURCE_DIR}
        ${REDISCPP_HEADER}
        ${HIREDIS_HEADER}
        ${REDIS_PROJECT_DIR}
        ${simple_common_base_SOURCE_DIR}
        ${JSON_INCLUDE_DIR}
        )
target_link_libraries(hashredisclient_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(hashredisclient_test
        PRIVATE
        pthread
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
        simple_color
        simple_config
        simple_logger
        simple_redis
        simple_common
        )

add_executable(fiforedisclient_test fiforedisclient_test.cpp)
target_include_directories(fiforedisclient_test
        PRIVATE
        .
        ${catch2_SOURCE_DIR}
        ${REDISCPP_HEADER}
        ${HIREDIS_HEADER}
        ${REDIS_PROJECT_DIR}
        ${simple_common_base_SOURCE_DIR}
        ${JSON_INCLUDE_DIR}
        )
target_link_libraries(fiforedisclient_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(fiforedisclient_test
        PRIVATE
        pthread
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
        simple_color
        simple_config
        simple_logger
        simple_redis
        simple_common
        )

add_executable(liforedisclient_test liforedisclient_test.cpp)
target_include_directories(liforedisclient_test
        PRIVATE
        .
        ${catch2_SOURCE_DIR}
        ${REDISCPP_HEADER}
        ${HIREDIS_HEADER}
        ${REDIS_PROJECT_DIR}
        ${simple_common_base_SOURCE_DIR}
        ${JSON_INCLUDE_DIR}
        )
target_link_libraries(liforedisclient_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(liforedisclient_test
        PRIVATE
        pthread
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
        simple_color
        simple_config
        simple_logger
        simple_redis
        simple_common
        )

add_executable(setredisclient_test setredisclient_test.cpp)
target_include_directories(setredisclient_test
        PRIVATE
        .
        ${catch2_SOURCE_DIR}
        ${REDISCPP_HEADER}
        ${HIREDIS_HEADER}
        ${REDIS_PROJECT_DIR}
        ${simple_common_base_SOURCE_DIR}
        ${JSON_INCLUDE_DIR}
        )
target_link_libraries(setredisclient_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(setredisclient_test
        PRIVATE
        pthread
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
        simple_color
        simple_config
        simple_logger
        simple_redis
        simple_common
        )