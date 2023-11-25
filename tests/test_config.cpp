//
// Created by Joaquin Bejar Garcia on 15/2/23.
//
#include <catch2/catch_test_macros.hpp>
#include <simple_redis/config.h>

simple_redis::RedisConfig default_config;

TEST_CASE("Declare RedisConfig", "[RedisDB]") {

    REQUIRE(default_config.to_string() ==
            R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})");
}

struct RedisEnvFixtureHostname {
    RedisEnvFixtureHostname() {
        setenv("REDIS_HOSTNAME", "other", 1);
    }

    ~RedisEnvFixtureHostname() {
        unsetenv("REDIS_HOSTNAME");
    }
};

TEST_CASE_METHOD(RedisEnvFixtureHostname, "Declare RedisConfig use HOST", "[RedisDB]") {
    setenv("REDIS_HOSTNAME", "other", 1);
    simple_redis::RedisConfig config;
    REQUIRE(config.to_string() ==
            R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"other","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})");
}

struct RedisEnvFixtureHostnames {
    RedisEnvFixtureHostnames() {
        setenv("REDIS_HOSTNAME", "other1,other2,other3", 1);
    }

    ~RedisEnvFixtureHostnames() {
        unsetenv("REDIS_HOSTNAME");
    }
};

TEST_CASE_METHOD(RedisEnvFixtureHostnames, "Declare RedisConfig use HOSTS", "[RedisDB]") {
    simple_redis::RedisConfig config;
    std::string expected1 = R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"other1","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})";
    std::string expected2 = R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"other2","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})";
    std::string expected3 = R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"other3","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})";
    REQUIRE((config.to_string() == expected1 || config.to_string() == expected2 || config.to_string() == expected3));
}

struct RedisEnvFixtureHostnamePortDatabase {
    RedisEnvFixtureHostnamePortDatabase() {
        setenv("REDIS_HOSTNAME", "localhost", 1);
        setenv("REDIS_PORT", "3306", 1);
        setenv("REDIS_DATABASE", "database", 1);
    }

    ~RedisEnvFixtureHostnamePortDatabase() {
        unsetenv("REDIS_HOSTNAME");
        unsetenv("REDIS_PORT");
        unsetenv("REDIS_DATABASE");
    }
};

TEST_CASE_METHOD(RedisEnvFixtureHostnamePortDatabase, "RedisConfig use to_string", "[RedisDB]") {
    simple_redis::RedisConfig config;
    REQUIRE(config.to_string() ==
            R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":3306,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})");
}

struct RedisEnvFixturePort {
    RedisEnvFixturePort() {
        setenv("REDIS_PORT", "3333", 1);
    }

    ~RedisEnvFixturePort() {
        unsetenv("REDIS_PORT");
    }
};

TEST_CASE_METHOD(RedisEnvFixturePort, "RedisConfig use to_string port", "[RedisDB]") {
    setenv("REDIS_PORT", "3333", 1);
    simple_redis::RedisConfig config;
    REQUIRE(config.to_string() ==
            R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":3333,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})");
}

TEST_CASE("RedisConfig validate", "[RedisDB]") {
    REQUIRE(default_config.validate());
}

struct RedisEnvFixtureTag {
    RedisEnvFixtureTag() {
        setenv("REDIS_TAG", "", 1);
    }

    ~RedisEnvFixtureTag() {
        unsetenv("REDIS_TAG");
    }
};

TEST_CASE_METHOD(RedisEnvFixtureTag, "RedisConfig validate wrong 1", "[RedisDB]") {
    simple_redis::RedisConfig config;
    REQUIRE_FALSE(config.validate());
    unsetenv("REDIS_TAG");
}

struct RedisEnvFixtureWrongPort {
    RedisEnvFixtureWrongPort() {
        setenv("REDIS_PORT", "1111111", 1);
    }

    ~RedisEnvFixtureWrongPort() {
        unsetenv("REDIS_PORT");
    }
};

TEST_CASE_METHOD(RedisEnvFixtureWrongPort, "RedisConfig validate wrong 2", "[RedisDB]") {

    simple_redis::RedisConfig config;
    REQUIRE_FALSE(config.validate());
    unsetenv("REDIS_PORT");
}

struct RedisEnvFixtureWrongTimeout {
    RedisEnvFixtureWrongTimeout() {
        setenv("REDIS_CONNECT_TIMEOUT", "-10", 1);
    }

    ~RedisEnvFixtureWrongTimeout() {
        unsetenv("REDIS_CONNECT_TIMEOUT");
    }
};

TEST_CASE_METHOD(RedisEnvFixtureWrongTimeout,"RedisConfig validate wrong 3", "[RedisDB]") {
    simple_redis::RedisConfig config;
    REQUIRE_FALSE(config.validate());
    unsetenv("REDIS_CONNECT_TIMEOUT");
}


TEST_CASE( "RedisConfig to json", "[RedisDB]") {
    json j = default_config.to_json();
    REQUIRE(j["host"] == "localhost");
    REQUIRE(j["port"] == 6379);
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