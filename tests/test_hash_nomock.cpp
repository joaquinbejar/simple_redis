//
// Created by Joaquin Bejar Garcia on 21/2/23.
//

#include <catch2/catch_test_macros.hpp>
#include <simple_redis/hash.h>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <utility>
#include <common/common.h>

simple_redis::RedisConfig global_config;

// REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS

TEST_CASE("HashRedisClient rule of 5 connect", "[RedisDB]") {
    simple_redis::HashRedisClient obj1(global_config);
    obj1.connect();  // fail if connect to a slave
    REQUIRE(obj1.is_connected());
}

TEST_CASE("HashRedisClient simple set", "[RedisDB]") {
    simple_redis::HashRedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = common::key_generator();
    obj1.del(key);
    REQUIRE(obj1.set(key, "key1", "value1"));
    REQUIRE(obj1.del(key));
}

TEST_CASE("HashRedisClient std::map set", "[RedisDB]") {
    simple_redis::HashRedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::map<std::string, std::string> map;
    map["key1"] = "value1";
    map["key2"] = "value2";
    map["key3"] = "value3";
    map["key4"] = "value4";
    std::string key = common::key_generator();
    obj1.del(key);
    REQUIRE(obj1.set(key, map));
    REQUIRE(obj1.del(key));
}

TEST_CASE("HashRedisClient simple set and delete", "[RedisDB]") {
    simple_redis::HashRedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = common::key_generator();
    obj1.del(key);
    REQUIRE(obj1.set(key, "key1", "value1"));
    REQUIRE(obj1.del(key));
}

TEST_CASE("HashRedisClient several set and delete", "[RedisDB]") {
    simple_redis::HashRedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = common::key_generator();
    obj1.del(key);
    obj1.del(key + "2");
    REQUIRE(obj1.set(key, "key1", "value1"));
    REQUIRE(obj1.set(key + "2", "key2", "value2"));
    REQUIRE(obj1.del(key));
    REQUIRE(obj1.del(key + "2"));

    REQUIRE(obj1.set(key, "key1", "value1"));
    REQUIRE(obj1.set(key + "2", "key2", "value2"));

    std::vector<std::string> keys;
    keys.push_back(key);
    keys.push_back(key + "2");
    REQUIRE(obj1.del(keys));
}

TEST_CASE("HashRedisClient simple get", "[RedisDB]") {
    simple_redis::HashRedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::map<std::string, std::string> map;
    map["key1"] = "value1";
    map["key2"] = "value2";
    map["key3"] = "value3";
    map["key4"] = "value4";
    std::string key = common::key_generator();
    obj1.del(key);
    REQUIRE(obj1.set(key, map));
    std::string value = obj1.get(key, "key1");
    REQUIRE(value == "value1");
    value = obj1.get(key, "key2");
    REQUIRE(value == "value2");
    value = obj1.get(key, "key3");
    REQUIRE(value == "value3");
    value = obj1.get(key, "key4");
    REQUIRE(value == "value4");
    REQUIRE(obj1.del(key));
}

TEST_CASE("HashRedisClient simple getall", "[RedisDB]") {
    simple_redis::HashRedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::map<std::string, std::string> map;
    map["key1"] = "value1";
    map["key2"] = "value2";
    map["key3"] = "value3";
    map["key4"] = "value4";
    std::string key = common::key_generator();
    obj1.del(key);
    REQUIRE(obj1.set(key, map));
    std::map<std::string ,std::string> value = obj1.get(key );
    REQUIRE(value == map);
    REQUIRE(obj1.del(key));
}

TEST_CASE("HashRedisClient getkeys", "[RedisDB]") {
    simple_redis::HashRedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::map<std::string, std::string> map;
    map["key1"] = "value1";
    map["key2"] = "value2";
    map["key3"] = "value3";
    map["key4"] = "value4";
    std::string key = common::key_generator();
    obj1.del(key);
    REQUIRE(obj1.set(key, map));
    std::vector<std::string> value = obj1.getkeys(key );
    REQUIRE(value.size() == 4);
    REQUIRE(value[0] == "key1");
    REQUIRE(value[1] == "key2");
    REQUIRE(value[2] == "key3");
    REQUIRE(value[3] == "key4");
    REQUIRE(obj1.del(key));
}

TEST_CASE("HashRedisClient delkey", "[RedisDB]") {
    simple_redis::HashRedisClient obj1(global_config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::map<std::string, std::string> map;
    map["key1"] = "value1";
    map["key2"] = "value2";
    map["key3"] = "value3";
    map["key4"] = "value4";
    std::string key = common::key_generator();
    obj1.del(key);
    REQUIRE(obj1.set(key, map));
    REQUIRE(obj1.delkey(key, "key1"));
    REQUIRE(obj1.delkey(key, "key3"));
    std::vector<std::string> value = obj1.getkeys(key );
    REQUIRE(value.size() == 2);
    REQUIRE(value[0] == "key2");
    REQUIRE(value[1] == "key4");
    REQUIRE(obj1.del(key));
}