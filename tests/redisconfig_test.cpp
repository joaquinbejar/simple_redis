//
// Created by Joaquin Bejar Garcia on 15/2/23.
//

#include <simple_config/config.h>
#include <catch2/catch_test_macros.hpp>
//#include <simple_redis/redis.h>
#include <simple_redis/config.h>


TEST_CASE("Declare RedisConfig", "[RedisDB]") {
    unsetenv("REDIS_HOSTNAME");
    unsetenv("REDIS_PORT");
    simple_redis::RedisConfig config;
    REQUIRE(config.to_string() ==
            R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})");
}

TEST_CASE("Declare RedisConfig use HOST", "[RedisDB]") {
    setenv("REDIS_HOSTNAME", "other", 1);
    unsetenv("REDIS_PORT");
    simple_redis::RedisConfig config;
    REQUIRE(config.to_string() ==
            R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"other","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})");
}

TEST_CASE("Declare RedisConfig use HOSTS", "[RedisDB]") {
    setenv("REDIS_HOSTNAME", "other1,other2,other3", 1);
    unsetenv("REDIS_PORT");
    simple_redis::RedisConfig config;
    std::string expected1 = R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"other1","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})";
    std::string expected2 = R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"other2","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})";
    std::string expected3 = R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"other3","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})";
    REQUIRE((config.to_string() == expected1 || config.to_string() == expected2 || config.to_string() == expected3));
}

TEST_CASE("RedisConfig use to_string", "[RedisDB]") {
    setenv("REDIS_HOSTNAME", "localhost", 1);
    setenv("REDIS_PORT", "3306", 1);
    setenv("REDIS_DATABASE", "database", 1);
    simple_redis::RedisConfig config;
    REQUIRE(config.to_string() ==
            R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":3306,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})");
}

TEST_CASE("RedisConfig use to_string port", "[RedisDB]") {
    setenv("REDIS_PORT", "3333", 1);
    unsetenv("REDIS_HOSTNAME");
    simple_redis::RedisConfig config;
    REQUIRE(config.to_string() ==
            R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":3333,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})");
}

TEST_CASE("RedisConfig validate", "[RedisDB]") {
    simple_redis::RedisConfig config;
    REQUIRE(config.validate());
}

TEST_CASE("RedisConfig validate wrong 1", "[RedisDB]") {
    setenv("REDIS_TAG", "", 1);
    simple_redis::RedisConfig config;
    REQUIRE_FALSE(config.validate());
    unsetenv("REDIS_TAG");
}

TEST_CASE("RedisConfig validate wrong 2", "[RedisDB]") {
    setenv("REDIS_PORT", "1111111", 1);
    simple_redis::RedisConfig config;
    REQUIRE_FALSE(config.validate());
    unsetenv("REDIS_PORT");
}

TEST_CASE("RedisConfig validate wrong 3", "[RedisDB]") {
    setenv("REDIS_CONNECT_TIMEOUT", "-10", 1);
    simple_redis::RedisConfig config;
    REQUIRE_FALSE(config.validate());
    unsetenv("REDIS_CONNECT_TIMEOUT");
}

TEST_CASE("RedisConfig check is_moved", "[RedisDB]") {
    setenv("REDIS_HOSTNAME", "localhost", 1);
    setenv("REDIS_PORT", "3333", 1);
    simple_redis::RedisConfig obj1;
    REQUIRE_FALSE(obj1.is_moved());
    simple_redis::RedisConfig obj4(std::move(obj1));
    REQUIRE(obj1.is_moved());
}

TEST_CASE("Declare RedisConfig rule of 5", "[RedisDB]") {
    setenv("REDIS_HOSTNAME", "localhost", 1);
    setenv("REDIS_PORT", "3333", 1);
    simple_redis::RedisConfig obj1;
    REQUIRE(obj1.validate());
    simple_redis::RedisConfig obj2(obj1);
    REQUIRE(obj2.validate());
    simple_redis::RedisConfig obj3 = obj1;
    REQUIRE(obj1.validate());
    REQUIRE(obj3.validate());
    simple_redis::RedisConfig obj4(std::move(obj1));
    REQUIRE(obj4.validate());
    REQUIRE_FALSE(obj1.validate());
    simple_redis::RedisConfig obj5 = std::move(obj2);
    REQUIRE(obj5.validate());
    REQUIRE_FALSE(obj2.validate());
    REQUIRE(obj5.to_string() ==
            R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":3333,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})");
    REQUIRE(obj4.to_string() ==
            R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":3333,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})");
    REQUIRE(obj3.to_string() ==
            R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":3333,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})");
    REQUIRE(obj5.logger == obj4.logger);
    REQUIRE(obj5.logger == obj3.logger);
}

TEST_CASE("RedisConfig to json", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    simple_redis::RedisConfig config;
    json j = config.to_json();
    REQUIRE(j["host"] == "localhost");
    REQUIRE(j["port"] == 3333);
    REQUIRE(j["db"] == 0);
    REQUIRE(j["password"] == "password");
    REQUIRE(j["size"] == 1000);
    REQUIRE(j["connect_timeout"] == 30000);
    REQUIRE(j["socket_timeout"] == 30000);
    REQUIRE(j["connection_idle_time"] == 0);
    REQUIRE(j["connection_lifetime"] == 0);
    REQUIRE(j["wait_timeout"] == 30000);
    REQUIRE(j["keep_alive"] == true);
    REQUIRE(j["tag"] == "tag");
    REQUIRE(j["loglevel"] == "info");
}

TEST_CASE("RedisConfig from json", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    simple_redis::RedisConfig config;
    json j = config.to_json();
    simple_redis::RedisConfig config2(j);
    REQUIRE(config == config2);
    simple_redis::RedisConfig config3;
    config3.from_json(j);
    REQUIRE(config == config3);
}

TEST_CASE("ConnectionOptions instantiate", "[RedisDB]") {
    simple_redis::ClusterConnectionOptions options;
    std::string value = R"({"connect_timeout":0,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"","keep_alive":false,"password":"","port":6379,"size":1,"socket_timeout":0,"tag":"tag","wait_timeout":0})";
    REQUIRE(options.to_string() == value);
    json j = options.to_json();
    REQUIRE(j["tag"] == "tag");
    REQUIRE(j["connect_timeout"] == 0);
    REQUIRE(j["connection_idle_time"] == 0);
    REQUIRE(j["connection_lifetime"] == 0);
    REQUIRE(j["db"] == 0);
    REQUIRE(j["host"] == "");
    REQUIRE(j["password"] == "");
    REQUIRE(j["port"] == 6379);
    REQUIRE(j["size"] == 1);
    REQUIRE(j["socket_timeout"] == 0);
    REQUIRE(j["keep_alive"] == false);
    REQUIRE(j["wait_timeout"] == 0);
}

TEST_CASE("ConnectionOptions rule of 5", "[RedisDB]") {
    simple_redis::ClusterConnectionOptions obj1;
    simple_redis::ClusterConnectionOptions obj2(obj1);
    simple_redis::ClusterConnectionOptions obj3 = obj1;
    simple_redis::ClusterConnectionOptions obj4(std::move(obj1));
    simple_redis::ClusterConnectionOptions obj5 = std::move(obj2);
    REQUIRE(obj5 == obj4);
    REQUIRE(obj4 == obj3);
}

TEST_CASE("ConnectionOptions from json error", "[RedisDB]") {
    json j = R"({"connect_timeout":0,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"","password":"","port":6379,"size":1,"socket_timeout":0,"keep_alive":false,"wait_timeout":0})"_json;
    simple_redis::ClusterConnectionOptions obj1;
    REQUIRE_THROWS(obj1.from_json(j));
}

TEST_CASE("ConnectionOptions from json", "[RedisDB]") {
    json j = R"({"connect_timeout":0,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"","password":"","port":6379,"size":1,"socket_timeout":0,"keep_alive":false,"wait_timeout":0,"tag":"tag"})"_json;
    simple_redis::ClusterConnectionOptions obj1;
    obj1.from_json(j);
    REQUIRE(obj1.to_string() == j.dump());
}