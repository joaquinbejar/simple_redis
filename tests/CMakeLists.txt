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

