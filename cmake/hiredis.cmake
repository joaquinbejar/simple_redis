include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

FetchContent_Declare(hiredis
        GIT_REPOSITORY https://github.com/redis/hiredis.git
        GIT_TAG v1.2.0
        CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF
        BUILD_ALWAYS 1
)
FetchContent_MakeAvailable(hiredis)

set(HIREDIS_INCLUDE ${hiredis_SOURCE_DIR} CACHE INTERNAL "")
set(HIREDIS_HEADER "${hiredis_SOURCE_DIR}" CACHE INTERNAL "")
set(HIREDIS_LIB "${hiredis_BINARY_DIR}/libhiredis.a" CACHE INTERNAL "")
set(TEST_HIREDIS_LIB "${hiredis_BINARY_DIR}/libhiredis.a" CACHE INTERNAL "")

if (CMAKE_DEBUG)
    message(STATUS "simple_mariadb/cmake hiredis_SOURCE_DIR ${hiredis_SOURCE_DIR}")
    message(STATUS "simple_mariadb/cmake HIREDIS_INCLUDE ${HIREDIS_INCLUDE}")
endif ()


if(NOT EXISTS "${HIREDIS_LIB}")
    message(STATUS "hiredis library not found, building it")
    execute_process(
            COMMAND ${CMAKE_COMMAND} -DBUILD_SHARED_LIBS=OFF -S ${hiredis_SOURCE_DIR} -B ${hiredis_BINARY_DIR}
            WORKING_DIRECTORY ${hiredis_SOURCE_DIR}
            COMMAND_ECHO STDOUT
            COMMAND_ECHO STDERR
            RESULT_VARIABLE result
    )
    if (result)
        message(FATAL_ERROR "Error Configuring hiredis ${result}")
    endif ()

    execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${hiredis_BINARY_DIR} --target hiredis
            WORKING_DIRECTORY ${hiredis_SOURCE_DIR}
            COMMAND_ECHO STDOUT
            COMMAND_ECHO STDERR
            RESULT_VARIABLE result
    )
    if (result)
        message(FATAL_ERROR "Error building hiredis ${result}")
    endif ()

    execute_process(
            COMMAND ln -sf ${hiredis_SOURCE_DIR} ${hiredis_SOURCE_DIR}/hiredis
            WORKING_DIRECTORY ${hiredis_SOURCE_DIR}
            COMMAND_ECHO STDOUT
            COMMAND_ECHO STDERR
            RESULT_VARIABLE result
    )
    if (result)
        message(FATAL_ERROR "Error linking hiredis ${result}")
    endif ()
else()
    message(STATUS "hiredis library found")
endif()



