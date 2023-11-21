//
// Created by Joaquin Bejar Garcia on 21/2/23.
//


#include <catch2/catch_test_macros.hpp>
#include <simple_redis/set.h>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <utility>
#include <thread>
#include <simple_common/common.h>

std::string key_generator(int size = 20, int each = 5) {
    std::string abc = "abcdefghijklmnopqrstuvwxyz";
    std::string key;

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < size; i++) {
        if (i > 0 && i % each == 0) {
            key += "_";
        }
        std::uniform_int_distribution<> distribucion(0, (int) abc.size() - 1);
        int indice = distribucion(gen);
        key += abc[indice];
    }
    return key;
}

simple_redis::RedisConfig global_config;
std::string value1 = "value1";
std::string value2 = "value2";


TEST_CASE("Declare SetRedisClient", "[RedisDB]") {
    simple_redis::RedisConfig config;
    simple_redis::SetRedisClient obj1(config);
    REQUIRE_FALSE(obj1.is_connected());
}

TEST_CASE("SetRedisClient rule of 5 connect", "[RedisDB]") {
    // REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS
    REQUIRE_FALSE(global_config.to_string() ==
                  R"({"connect_timeout":3000,"connection_idle_time":3000,"connection_lifetime":3000,"db":0,"host":"localhost","keep_alive":true,"password":"password","port":6379,"size":1000,"socket_timeout":3000,"tag":"default","wait_timeout":3000})");
    simple_redis::SetRedisClient obj1(global_config);
    obj1.connect();  // fail if connect to a slave
    REQUIRE(obj1.is_connected());
    simple_redis::SetRedisClient obj2(obj1);
    obj2.connect();
    REQUIRE(obj2.is_connected());
    simple_redis::SetRedisClient obj3(std::move(obj1));
    obj3.connect();
    REQUIRE(obj3.is_connected());
    simple_redis::SetRedisClient obj4 = obj2;
    obj4.connect();
    REQUIRE(obj4.is_connected());
    simple_redis::SetRedisClient obj5 = std::move(obj2);
    obj5.connect();
    REQUIRE(obj5.is_connected());
}

TEST_CASE("SetRedisClient simple set", "[RedisDB]") {
    std::string tag = "I_am_a_set_set_" + key_generator(4, 2);
    setenv("REDIS_TAG", tag.c_str(), 1);
    simple_redis::RedisConfig config;
    simple_redis::SetRedisClient obj1(config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = "key";
    obj1.del(key);

    REQUIRE(obj1.set(key, value1));
    REQUIRE(obj1.set(key, value2));
    REQUIRE(obj1.set(key, value1));
    REQUIRE(obj1.set(key + "_2", value1));

    REQUIRE(obj1.del(key));
    REQUIRE(obj1.del(key + "_2"));
}

TEST_CASE("SetRedisClient simple get", "[RedisDB]") {
    std::string tag = "I_am_a_set_get_" + key_generator(4, 2);
    setenv("REDIS_TAG", tag.c_str(), 1);
    simple_redis::RedisConfig config;
    simple_redis::SetRedisClient obj1(config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = "key";
    obj1.del(key);
    REQUIRE(obj1.set(key, value1));
    std::string value = obj1.get(key);
    REQUIRE(value == value1);
    REQUIRE(obj1.del(key));
}

TEST_CASE("SetRedisClient simple missing key", "[RedisDB]") {
    std::string tag = "I_am_a_set_missing_" + key_generator(4, 2);
    setenv("REDIS_TAG", tag.c_str(), 1);
    simple_redis::RedisConfig config;
    simple_redis::SetRedisClient obj1(config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = "key";
    obj1.del(key);
    std::string value = obj1.get(key);
    REQUIRE(value.empty());
    value = obj1.get(key, "default_value");
    REQUIRE(value == "default_value");
    REQUIRE_FALSE(obj1.del(key));
}

TEST_CASE("SetRedisClient set with ttl", "[RedisDB]") {
    std::string tag = "I_am_a_set_set_ttl_" + key_generator(4, 2);
    setenv("REDIS_TAG", tag.c_str(), 1);
    simple_redis::RedisConfig config;
    simple_redis::SetRedisClient obj1(config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::string key = "key";
    obj1.del(key);
    REQUIRE(obj1.set(key, value1, 5));
    std::string value = obj1.get(key);
    REQUIRE(value == "value1");
    std::this_thread::sleep_for(std::chrono::seconds(6));
    value = obj1.get(key);
    REQUIRE(value.empty());
}

TEST_CASE("SetRedisClient set map", "[RedisDB]") {
    std::string tag = "I_am_a_set_set_map_" + key_generator(4, 2);
    setenv("REDIS_TAG", tag.c_str(), 1);
    simple_redis::RedisConfig config;
    simple_redis::SetRedisClient obj1(config);
    obj1.connect();
    REQUIRE(obj1.is_connected());

    std::map<std::string, std::string> m;
    m["key1"] = "value1";
    m["key2"] = "value2";
    m["key3"] = "value3";
    m["key4"] = "value4";
    // turn map into a set
    std::set<std::string> s;
    for (auto &i: m) {
        s.insert(i.first);
    }
    REQUIRE(obj1.set(m));
    REQUIRE(obj1.del(s));
}

TEST_CASE("SetRedisClient set size", "[RedisDB]") {
    std::string tag = "I_am_a_set_size_" + key_generator(4, 2);
    setenv("REDIS_TAG", tag.c_str(), 1);
    simple_redis::RedisConfig config;
    simple_redis::SetRedisClient obj1(config);
    obj1.connect();
    REQUIRE(obj1.is_connected());

    std::map<std::string, std::string> m;
    m["key1"] = "value1";
    m["key2"] = "value2";
    m["key3"] = "value3";
    m["key4"] = "value4";
    // turn map into a set
    std::set<std::string> s;
    for (auto &i: m) {
        s.insert(i.first);
    }
    REQUIRE(obj1.set(m));
    auto all_keys = obj1.get_keys();
    REQUIRE(!all_keys.empty()); // maybe 0 if the key is not set in that node
    REQUIRE(obj1.del(s));
}

TEST_CASE("SetRedisClient set json", "[RedisDB]") {
    std::string tag = "I_am_a_set_json_" + key_generator(4, 2);
    setenv("REDIS_TAG", tag.c_str(), 1);
    simple_redis::RedisConfig config;
    simple_redis::SetRedisClient obj1(config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    json j = json::parse(R"(
    {
        "key1": "value1",
        "key2": "value2",
        "key3": "value3",
        "key4": "value4",
        "key5": {
            "key6": "value6",
            "key7": "value7"
        }
    }
    )");
    REQUIRE(obj1.set("json", j.dump()));
    json j2 = json::parse(obj1.get("json"));
    REQUIRE(j == j2);
    REQUIRE(obj1.del("json"));
}

TEST_CASE("SetRedisClient clear", "[RedisDB]") {
    std::string tag = "I_am_a_set_clear_" + key_generator(4, 2);
    setenv("REDIS_TAG", tag.c_str(), 1);
    simple_redis::RedisConfig config;
    simple_redis::SetRedisClient obj1(config);
    obj1.connect();
    REQUIRE(obj1.is_connected());

    std::map<std::string, std::string> m;
    m["key1"] = "value1";
    m["key2"] = "value2";
    m["key3"] = "value3";
    m["key4"] = "value4";
    // turn map into a set
    std::set<std::string> s;
    for (auto &i: m) {
        s.insert(i.first);
    }
    REQUIRE(obj1.set(m));
    auto all_keys = obj1.get_keys();
    REQUIRE(obj1.clear());

    all_keys = obj1.get_keys();
    REQUIRE(all_keys.empty());
    REQUIRE(obj1.size() == 0);
}

TEST_CASE("SetRedisClient set and delete all", "[RedisDB]") {
    std::string tag = "I_am_a_set_delete_all_" + key_generator(4, 2);
    setenv("REDIS_TAG", tag.c_str(), 1);
    simple_redis::RedisConfig config;
    simple_redis::SetRedisClient obj1(config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    std::vector<std::string> keys;
    // insert 1000 keys
    for (int i = 0; i < 100; i++) {
        std::string key = simple_common::key_generator();
        std::string value = "value_" + key;
        REQUIRE(obj1.set(key, value));
        keys.push_back(key);
    }
    // delete all keys
    REQUIRE(obj1.del(keys));
}

TEST_CASE("SetRedisClient set and delete all from keys *", "[RedisDB]") {
    std::string tag = "I_am_a_delete_all_from_keys_" + key_generator(4, 2);
    setenv("REDIS_TAG", tag.c_str(), 1);
    simple_redis::RedisConfig config;
    simple_redis::SetRedisClient obj1(config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    for (int i = 0; i < 100; i++) {
        std::string key = simple_common::key_generator();
        std::string value = "value_" + key;
        REQUIRE(obj1.set(key, value));
    }
    auto keys = obj1.get_keys();
    REQUIRE(!keys.empty());
    REQUIRE(keys.size() == 100);
    REQUIRE(obj1.del(keys));
}

TEST_CASE("SetRedisClient get size", "[RedisDB]") {
    std::string tag = "I_am_a_get_size_" + key_generator(4, 2);
    setenv("REDIS_TAG", tag.c_str(), 1);
    simple_redis::RedisConfig config;
    simple_redis::SetRedisClient obj1(config);
    obj1.connect();
    REQUIRE(obj1.is_connected());
    for (int i = 0; i < 100; i++) {
        std::string key = simple_common::key_generator();
        std::string value = "value_" + key;
        REQUIRE(obj1.set(key, value));
    }
    REQUIRE(obj1.size() == 100);
    REQUIRE(obj1.clear());
    REQUIRE(obj1.size() == 0);
}
