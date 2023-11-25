//
// Created by Joaquin Bejar Garcia on 21/2/23.
//

#include <catch2/catch_test_macros.hpp>
#include <simple_redis/fifo.h>
#include <catch2/benchmark/catch_benchmark.hpp>

simple_redis::RedisConfig global_config;

TEST_CASE("Declare FIFORedisClient", "[RedisDB]") {
    simple_redis::FIFORedisClient obj1(global_config);
    REQUIRE_FALSE(obj1.is_connected());
}


