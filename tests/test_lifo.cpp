//
// Created by Joaquin Bejar Garcia on 21/2/23.
//

#include <catch2/catch_test_macros.hpp>
#include <simple_redis/lifo.h>
#include <catch2/benchmark/catch_benchmark.hpp>


TEST_CASE("Declare LIFORedisClient", "[RedisDB]") {
    simple_redis::RedisConfig config;
    simple_redis::LIFORedisClient obj1(config);
    REQUIRE_FALSE(obj1.is_connected());
}


