//
// Created by Joaquin Bejar Garcia on 15/2/23.
//

#include <catch2/catch_test_macros.hpp>
#include <simple_redis/client.h>
#include <simple_redis/config.h>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <utility>
#include <thread>

simple_redis::RedisConfig global_config;


TEST_CASE("Declare RedisClient", "[RedisDB]") {
    std::string expected = R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})";
    REQUIRE(global_config.to_string() == expected);
    simple_redis::RedisClient obj1(global_config);
    REQUIRE_FALSE(obj1.is_connected());
}

TEST_CASE("RedisClient rule of 5 NO connect", "[RedisDB]") {
    std::string expected = R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})";
    REQUIRE(global_config.to_string() == expected);
    simple_redis::RedisClient obj1(global_config);
    REQUIRE_THROWS(obj1.connect());
}

TEST_CASE("RedisClient get without connect", "[RedisDB]") {
    simple_redis::RedisClient obj1(global_config);
    REQUIRE_FALSE(obj1.check_servers());
}