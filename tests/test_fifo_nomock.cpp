//
// Created by Joaquin Bejar Garcia on 21/2/23.
//

#include <catch2/catch_test_macros.hpp>
#include <simple_redis/fifo.h>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <utility>
#include <common/common.h>

simple_redis::RedisConfig global_config;

// REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS

TEST_CASE("FIFORedisClient rule of 5 connect", "[RedisDB]") {

    REQUIRE_FALSE(global_config.to_string() ==
                  R"({"connect_timeout":3000,"connection_idle_time":3000,"connection_lifetime":3000,"db":0,"host":"localhost","keep_alive":true,"password":"password","port":6379,"size":1000,"socket_timeout":3000,"tag":"default","wait_timeout":3000})");
    simple_redis::FIFORedisClient obj1(global_config);
    obj1.connect();  // fail if connect to a slave
    REQUIRE(obj1.is_connected());
}

TEST_CASE("FIFORedisClient simple set", "[RedisDB]") {
    simple_redis::FIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = common::key_generator();
    obj1.del(key);
    REQUIRE(obj1.set(key, "value1"));
    REQUIRE(obj1.set(key, "value2"));
    REQUIRE(obj1.set(key, "value1"));
    REQUIRE(obj1.set(key + "_2", "value1"));
    REQUIRE(obj1.del(key));
    REQUIRE(obj1.del(key + "_2"));
}

TEST_CASE("FIFORedisClient std::vector set", "[RedisDB]") {
    simple_redis::FIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::vector<std::string> v;
    v.emplace_back("value1");
    v.emplace_back("value2");
    v.emplace_back("value3");
    v.emplace_back("value4");

    std::string key = common::key_generator();
    obj1.del(key);
    REQUIRE(obj1.set(key, v));
    REQUIRE(obj1.set(key + "_2", v));
    REQUIRE(obj1.del(key));
    REQUIRE(obj1.del(key + "_2"));
}

TEST_CASE("FIFORedisClient simple get", "[RedisDB]") {
    simple_redis::FIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = common::key_generator();
    obj1.del(key);
    std::vector<std::string> v({"value1", "value2", "value3", "value4"});
    REQUIRE(obj1.set(key, v));
    std::string value = obj1.get(key);
    REQUIRE(value == "value1");
    value = obj1.get(key);
    REQUIRE(value == "value2");
    value = obj1.get(key);
    REQUIRE(value == "value3");
    REQUIRE(obj1.del(key));
}

TEST_CASE("FIFORedisClient simple get &&", "[RedisDB]") {
    simple_redis::FIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = common::key_generator();
    obj1.del(key);
    std::vector<std::string> v({"value1", "value2", "value3", "value4"});
    REQUIRE(obj1.set(key + "_2", v));
    std::string value = obj1.get(key + "_2");
    REQUIRE(value == "value1");
    value = obj1.get(key + "_2");
    REQUIRE(value == "value2");
    value = obj1.get(key + "_2");
    REQUIRE(value == "value3");
    REQUIRE(obj1.del(key + "_2"));
}

TEST_CASE("FIFORedisClient several get", "[RedisDB]") {
    simple_redis::FIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = common::key_generator();
    obj1.del(key);
    std::vector<std::string> v({"value1", "value2", "value3", "value4"});
    REQUIRE(obj1.set(key, v));
    std::vector<std::string> value = obj1.get(key, 3);
    REQUIRE(value.size() == 3);
    REQUIRE(value[0] == "value1");
    REQUIRE(value[1] == "value2");
    REQUIRE(value[2] == "value3");
    REQUIRE(obj1.del(key));
}

TEST_CASE("FIFORedisClient several get &&", "[RedisDB]") {
    simple_redis::FIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = common::key_generator();
    obj1.del(key);
    std::vector<std::string> v({"value1", "value2", "value3", "value4"});
    REQUIRE(obj1.set(key + "_2", v));
    std::vector<std::string> value = obj1.get(key + "_2", 3);
    REQUIRE(value.size() == 3);
    REQUIRE(value[0] == "value1");
    REQUIRE(value[1] == "value2");
    REQUIRE(value[2] == "value3");
    REQUIRE(obj1.del(key + "_2"));
}

TEST_CASE("FIFORedisClient size", "[RedisDB]") {
    simple_redis::FIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = common::key_generator();
    obj1.del(key);
    std::vector<std::string> v({"value1", "value2", "value3", "value4"});
    REQUIRE(obj1.set(key, v));
    REQUIRE(obj1.size(key) == 4);
    std::vector<std::string> value = obj1.get(key, 2);
    REQUIRE(value.size() == 2);
    REQUIRE(obj1.size(key) == 2);
    std::string value_string = obj1.get(key);
    REQUIRE(obj1.size(key) == 1);
    REQUIRE(obj1.del(key));
}

TEST_CASE("FIFORedisClient size &&", "[RedisDB]") {
    simple_redis::FIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = common::key_generator();
    obj1.del(key);
    std::vector<std::string> v({"value1", "value2", "value3", "value4"});
    REQUIRE(obj1.set(key, v));
    REQUIRE(obj1.size(key) == 4);
    std::vector<std::string> value = obj1.get(key, 2);
    REQUIRE(value.size() == 2);
    REQUIRE(obj1.size(key) == 2);
    std::string value_string = obj1.get(key);
    REQUIRE(obj1.size(key) == 1);
    REQUIRE(obj1.del(key));
}

TEST_CASE("FIFORedisClient simple NO BLOCKING get", "[RedisDB]") {
    simple_redis::FIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());

    SECTION("simple NO BLOCKING get") {
        std::string key = common::key_generator();
        obj1.del(key);
        REQUIRE(obj1.set(key, "value1"));
        std::string value = obj1.nb_get(key);
        REQUIRE(value == "value1");
    }

    SECTION("simple NO BLOCKING get &&") {
        std::string key = common::key_generator();
        obj1.del(key + "_2");
        REQUIRE(obj1.set(key + "_2", "value1"));
        std::string value = obj1.nb_get(key + "_2");
        REQUIRE(value == "value1");
    }

    SECTION("several NO BLOCKING get") {
        std::string key = common::key_generator();
        obj1.del(key);
        std::vector<std::string> v({"value1", "value2", "value3", "value4"});
        REQUIRE(obj1.set(key, v));
        std::vector<std::string> value = obj1.nb_get(key, 3);
        REQUIRE(value[0] == "value1");
        REQUIRE(value[1] == "value2");
        REQUIRE(value[2] == "value3");
    }

    SECTION("several NO BLOCKING get &&") {
        std::string key = common::key_generator();
        obj1.del(key + "_2");
        std::vector<std::string> v({"value1", "value2", "value3", "value4"});
        REQUIRE(obj1.set(key + "_2", v));
        std::vector<std::string> value = obj1.nb_get(key + "_2", 3);
        REQUIRE(value[0] == "value1");
        REQUIRE(value[1] == "value2");
        REQUIRE(value[2] == "value3");
    }

}

