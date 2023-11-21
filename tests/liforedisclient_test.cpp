//
// Created by Joaquin Bejar Garcia on 21/2/23.
//

#include <catch2/catch_test_macros.hpp>
#include <simple_redis/lifo.h>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <utility>
#include <simple_common/common.h>

simple_redis::RedisConfig global_config;

TEST_CASE("Declare LIFORedisClient", "[RedisDB]") {
    simple_redis::RedisConfig config;
    simple_redis::LIFORedisClient obj1(config);
    REQUIRE_FALSE(obj1.is_connected());
}

TEST_CASE("LIFORedisClient rule of 5 connect", "[RedisDB]") {
    // REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS
    simple_redis::LIFORedisClient obj1(global_config);
    obj1.connect();  // fail if connect to a slave
    REQUIRE(obj1.is_connected());
    simple_redis::LIFORedisClient obj2(obj1);
    obj2.connect();
    REQUIRE(obj2.is_connected());
    simple_redis::LIFORedisClient obj3(std::move(obj1));
    obj3.connect();
    REQUIRE(obj3.is_connected());
    simple_redis::LIFORedisClient obj4 = obj2;
    obj4.connect();
    REQUIRE(obj4.is_connected());
    simple_redis::LIFORedisClient obj5 = std::move(obj2);
    obj5.connect();
    REQUIRE(obj5.is_connected());
}

TEST_CASE("LIFORedisClient simple set", "[RedisDB]") {
    simple_redis::LIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = simple_common::key_generator();
    obj1.del(key);
    REQUIRE(obj1.set(key, "value1"));
    REQUIRE(obj1.set(key, "value2"));
    REQUIRE(obj1.set(key, "value1"));
    REQUIRE(obj1.set(key + "_2", "value1"));
    REQUIRE(obj1.del(key));
    REQUIRE(obj1.del(key + "_2"));
}

TEST_CASE("LIFORedisClient std::vector set", "[RedisDB]") {
    simple_redis::LIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::vector<std::string> v;
    v.emplace_back("value1");
    v.emplace_back("value2");
    v.emplace_back("value3");
    v.emplace_back("value4");

    std::string key = simple_common::key_generator();
    obj1.del(key);
    REQUIRE(obj1.set(key, v));
    REQUIRE(obj1.set(key + "_2", v));
    REQUIRE(obj1.del(key));
    REQUIRE(obj1.del(key + "_2"));
}

TEST_CASE("LIFORedisClient simple get", "[RedisDB]") {
    simple_redis::LIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = simple_common::key_generator();
    obj1.del(key);
    std::vector<std::string> v({"value1", "value2", "value3", "value4"});
    REQUIRE(obj1.set(key, v));
    std::string value = obj1.get(key);
    REQUIRE(value == "value4");
    value = obj1.get(key);
    REQUIRE(value == "value3");
    value = obj1.get(key);
    REQUIRE(value == "value2");
    REQUIRE(obj1.del(key));
}

TEST_CASE("LIFORedisClient simple get &&", "[RedisDB]") {
    simple_redis::LIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = simple_common::key_generator();
    obj1.del(key);
    std::vector<std::string> v({"value1", "value2", "value3", "value4"});
    REQUIRE(obj1.set(key + "_2", v));
    std::string value = obj1.get(key + "_2");
    REQUIRE(value == "value4");
    value = obj1.get(key + "_2");
    REQUIRE(value == "value3");
    value = obj1.get(key + "_2");
    REQUIRE(value == "value2");
    REQUIRE(obj1.del(key + "_2"));
}

TEST_CASE("LIFORedisClient several get", "[RedisDB]") {
    simple_redis::LIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = simple_common::key_generator();
    obj1.del(key);
    std::vector<std::string> v({"value1", "value2", "value3", "value4"});
    REQUIRE(obj1.set(key, v));
    std::vector<std::string> value = obj1.get(key, 3);
    REQUIRE(value.size() == 3);
    REQUIRE(value[0] == "value4");
    REQUIRE(value[1] == "value3");
    REQUIRE(value[2] == "value2");
    REQUIRE(obj1.del(key));
}

TEST_CASE("LIFORedisClient several get &&", "[RedisDB]") {
    simple_redis::LIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = simple_common::key_generator();
    obj1.del(key);
    std::vector<std::string> v({"value1", "value2", "value3", "value4"});
    REQUIRE(obj1.set(key + "_2", v));
    std::vector<std::string> value = obj1.get(key + "_2", 3);
    REQUIRE(value.size() == 3);
    REQUIRE(value[0] == "value4");
    REQUIRE(value[1] == "value3");
    REQUIRE(value[2] == "value2");
    REQUIRE(obj1.del(key + "_2"));
}

TEST_CASE("LIFORedisClient size", "[RedisDB]") {
    simple_redis::LIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = simple_common::key_generator();
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

TEST_CASE("LIFORedisClient size &&", "[RedisDB]") {
    simple_redis::LIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = simple_common::key_generator();
    obj1.del(key);
    std::vector<std::string> v({"value1", "value2", "value3", "value4"});
    REQUIRE(obj1.set(key + "2", v));
    REQUIRE(obj1.size(key + "2") == 4);
    std::vector<std::string> value = obj1.get(key + "2", 2);
    REQUIRE(value.size() == 2);
    REQUIRE(obj1.size(key + "2") == 2);
    std::string value_string = obj1.get(key + "2");
    REQUIRE(obj1.size(key + "2") == 1);
    REQUIRE(obj1.del(key + "2"));
}

TEST_CASE("FIFORedisClient simple NO BLOCKING get", "[RedisDB]") {
    simple_redis::LIFORedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());

    SECTION("simple NO BLOCKING get") {
        std::string key = simple_common::key_generator();
        obj1.del(key);
        REQUIRE(obj1.set(key, "value1"));
        std::string value = obj1.nb_get(key);
        REQUIRE(value == "value1");
    }

    SECTION("simple NO BLOCKING get &&") {
        std::string key = simple_common::key_generator();
        obj1.del(key + "_2");
        REQUIRE(obj1.set(key + "_2", "value1"));
        std::string value = obj1.nb_get(key + "_2");
        REQUIRE(value == "value1");
    }

    SECTION("several NO BLOCKING get") {
        std::string key = simple_common::key_generator();
        obj1.del(key);
        std::vector<std::string> v({"value1", "value2", "value3", "value4"});
        REQUIRE(obj1.set(key, v));
        std::vector<std::string> value = obj1.nb_get(key, 3);
        REQUIRE(value[0] == "value4");
        REQUIRE(value[1] == "value3");
        REQUIRE(value[2] == "value2");
    }

    SECTION("several NO BLOCKING get &&") {
        std::string key = simple_common::key_generator();
        obj1.del(key + "_2");
        std::vector<std::string> v({"value1", "value2", "value3", "value4"});
        REQUIRE(obj1.set(key + "_2", v));
        std::vector<std::string> value = obj1.nb_get(key + "_2", 3);
        REQUIRE(value[0] == "value4");
        REQUIRE(value[1] == "value3");
        REQUIRE(value[2] == "value2");
    }

}