//
// Created by Joaquin Bejar Garcia on 15/2/23.
//

#include <catch2/catch_test_macros.hpp>
#include <simple_redis/basic.h>
#include <simple_redis/config.h>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <utility>

simple_redis::RedisConfig global_config;


TEST_CASE("get_nodes_from_string space", "[Redis]") {
    std::string hostnames = "localhost localhost1 localhost2";
    std::string node = simple_redis::get_node_from_string(hostnames, ' ');

    REQUIRE((node == "localhost" || node == "localhost1" || node == "localhost2"));
}

TEST_CASE("get_nodes_from_string coma", "[Redis]") {
    std::string hostnames = "localhost,localhost1,localhost2";
    std::string node = simple_redis::get_node_from_string(hostnames, ',');

    REQUIRE((node == "localhost" || node == "localhost1" || node == "localhost2"));
}

TEST_CASE("get_nodes_from_string one host", "[Redis]") {
    std::string hostnames = "localhost";
    std::string node = simple_redis::get_node_from_string(hostnames, ',');

    REQUIRE(node == "localhost");
}

TEST_CASE("get_nodes_from_string wrong", "[Redis]") {
    std::string hostnames = "localhost localhost1 localhost2";
    std::string node = simple_redis::get_node_from_string(hostnames, ',');

    REQUIRE(node == "localhost localhost1 localhost2");

}

TEST_CASE("get_nodes_from_string empty", "[Redis]") {
    std::string hostnames;
    std::string node = simple_redis::get_node_from_string(hostnames, ',');
    REQUIRE(node == "localhost");
}

TEST_CASE("set tag empty", "[Redis]") {
    std::string tag;
    REQUIRE(simple_redis::set_tag(tag, "key") == "__empty_tag__:key");
}

TEST_CASE("del tag empty", "[Redis]") {
    REQUIRE(simple_redis::del_tag("key") == "key");
}

TEST_CASE("del tag empty __empty_tag__", "[Redis]") {
    REQUIRE(simple_redis::del_tag("__empty_tag__:key") == "key");
}

TEST_CASE("del tag  I_am_a_tag", "[Redis]") {
    REQUIRE(simple_redis::del_tag("__I_am_a_tag__:key") == "key");
}

TEST_CASE("del tag vector I_am_a_tag", "[Redis]") {

    std::vector<std::string> keys = {"__I_am_a_tag__:key1", "__I_am_a_tag__:key2", "__I_am_a_tag__:key3"};
    simple_redis::del_tag(keys);
    REQUIRE(keys == std::vector<std::string>{"key1", "key2", "key3"});
}