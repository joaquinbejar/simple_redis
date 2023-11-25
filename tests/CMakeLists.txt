if (CMAKE_DEBUG)
    message(STATUS "simple_redis/tests SIMPLE_COLOR_INCLUDE: ${SIMPLE_COLOR_INCLUDE}")
    message(STATUS "simple_redis/tests SIMPLE_CONFIG_INCLUDE: ${SIMPLE_CONFIG_INCLUDE}")
    message(STATUS "simple_redis/tests SIMPLE_LOGGER_INCLUDE: ${SIMPLE_LOGGER_INCLUDE}")
    message(STATUS "simple_redis/tests NLOHMANN_JSON_INCLUDE: ${NLOHMANN_JSON_INCLUDE}")
    message(STATUS "simple_redis/tests COMMON_INCLUDE: ${COMMON_INCLUDE}")
endif ()


add_executable(test_config test_config.cpp)
target_include_directories(test_config
        PRIVATE
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${REDISCPP_HEADER}
        ${REDISCPP_HEADER17}
        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}

)

target_link_libraries(test_config PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_config PRIVATE
        simple_logger
        simple_config
        common
        simple_redis
        #        ${REDISCPP_LIB}
        #        ${HIREDIS_LIB}
)

add_executable(test_client test_client.cpp)
target_include_directories(test_client
        PRIVATE
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${REDISCPP_HEADER}
        ${REDISCPP_HEADER17}
        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}

)

target_link_libraries(test_client PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_client PRIVATE
        simple_logger
        simple_config
        common
        simple_redis
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
)

add_executable(test_client_nomock test_client_nomock.cpp)
target_include_directories(test_client_nomock
        PRIVATE
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${REDISCPP_HEADER}
        ${REDISCPP_HEADER17}
        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}

)

target_link_libraries(test_client_nomock PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_client_nomock PRIVATE
        simple_logger
        simple_config
        common
        simple_redis
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
)

add_executable(test_fifo test_fifo.cpp)
target_include_directories(test_fifo
        PRIVATE
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${REDISCPP_HEADER}
        ${REDISCPP_HEADER17}
        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}

)

target_link_libraries(test_fifo PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_fifo PRIVATE
        simple_logger
        simple_config
        common
        simple_redis
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
)

add_executable(test_fifo_nomock test_fifo_nomock.cpp)
target_include_directories(test_fifo_nomock
        PRIVATE
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${REDISCPP_HEADER}
        ${REDISCPP_HEADER17}
        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}

)

target_link_libraries(test_fifo_nomock PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_fifo_nomock PRIVATE
        simple_logger
        simple_config
        common
        simple_redis
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
)

add_executable(test_lifo test_lifo.cpp)
target_include_directories(test_lifo
        PRIVATE
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${REDISCPP_HEADER}
        ${REDISCPP_HEADER17}
        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}

)

target_link_libraries(test_lifo PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_lifo PRIVATE
        simple_logger
        simple_config
        common
        simple_redis
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
)

add_executable(test_lifo_nomock test_lifo_nomock.cpp)
target_include_directories(test_lifo_nomock
        PRIVATE
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${REDISCPP_HEADER}
        ${REDISCPP_HEADER17}
        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}

)

target_link_libraries(test_lifo_nomock PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_lifo_nomock PRIVATE
        simple_logger
        simple_config
        common
        simple_redis
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
)



add_executable(test_hash test_hash.cpp)
target_include_directories(test_lifo
        PRIVATE
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${REDISCPP_HEADER}
        ${REDISCPP_HEADER17}
        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}

)

target_link_libraries(test_hash PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_hash PRIVATE
        simple_logger
        simple_config
        common
        simple_redis
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
)

add_executable(test_hash_nomock test_hash_nomock.cpp)
target_include_directories(test_hash_nomock
        PRIVATE
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${REDISCPP_HEADER}
        ${REDISCPP_HEADER17}
        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}

)

target_link_libraries(test_hash_nomock PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_hash_nomock
        PRIVATE
        simple_logger
        simple_config
        common
        simple_redis
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
)


add_executable(test_set test_set.cpp)
target_include_directories(test_set
        PRIVATE
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${REDISCPP_HEADER}
        ${REDISCPP_HEADER17}
        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}

)

target_link_libraries(test_set PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_set PRIVATE
        simple_logger
        simple_config
        common
        simple_redis
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
)

add_executable(test_set_nomock test_set_nomock.cpp)
target_include_directories(test_set_nomock
        PRIVATE
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${REDISCPP_HEADER}
        ${REDISCPP_HEADER17}
        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}
)

target_link_libraries(test_set_nomock PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_set_nomock
        PRIVATE
        simple_logger
        simple_config
        common
        simple_redis
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
)


add_executable(test_functions test_functions.cpp)
target_include_directories(test_functions
        PRIVATE
        ${SIMPLE_REDIS_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${REDISCPP_HEADER}
        ${REDISCPP_HEADER17}
        ${REDISCPP_HEADERTLS}
        ${HIREDIS_HEADER}

)

target_link_libraries(test_functions PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_functions PRIVATE
        simple_logger
        simple_config
        common
        simple_redis
        ${REDISCPP_LIB}
        ${HIREDIS_LIB}
)