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
    unsetenv("REDIS_PASSWORD");
    unsetenv("REDIS_HOSTNAME");
    unsetenv("REDIS_PORT");
    unsetenv("LOGLEVEL");
    unsetenv("REDIS_TAG");
    simple_redis::RedisConfig config;
    std::string expected = R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})";
    REQUIRE(config.to_string() == expected);
    simple_redis::RedisClient obj1(config);
    REQUIRE_FALSE(obj1.is_connected());
}

TEST_CASE("RedisClient rule of 5 NO connect", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    unsetenv("REDIS_PASSWORD");
    unsetenv("REDIS_HOSTNAME");
    unsetenv("REDIS_PORT");
    unsetenv("REDIS_TAG");
    simple_redis::RedisConfig config;
    std::string expected = R"({"RedisConfig":{"connect_timeout":30000,"connection_idle_time":0,"connection_lifetime":0,"db":0,"host":"localhost","keep_alive":true,"loglevel":"info","password":"password","port":6379,"size":1000,"socket_timeout":30000,"tag":"tag","wait_timeout":30000}})";
    REQUIRE(config.to_string() == expected);
    simple_redis::RedisClient obj1(config);
    REQUIRE_THROWS(obj1.connect());
    simple_redis::RedisClient obj2(obj1);
    REQUIRE_THROWS(obj2.connect());
    simple_redis::RedisClient obj3(std::move(obj1));
    REQUIRE_THROWS(obj3.connect());
    simple_redis::RedisClient obj4 = obj2;
    REQUIRE_THROWS(obj4.connect());
    simple_redis::RedisClient obj5 = std::move(obj2);
    REQUIRE_THROWS(obj5.connect());
}

TEST_CASE("RedisClient rule of 5 connect", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    // REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS
    simple_redis::RedisClient obj1(global_config);
    obj1.connect();  // fail if connect to a slave
    REQUIRE(obj1.is_connected());
    simple_redis::RedisClient obj2(obj1);
    obj2.connect();
    REQUIRE(obj2.is_connected());
    simple_redis::RedisClient obj3(std::move(obj1));
    obj3.connect();
    REQUIRE(obj3.is_connected());
    simple_redis::RedisClient obj4 = obj2;
    obj4.connect();
    REQUIRE(obj4.is_connected());
    simple_redis::RedisClient obj5 = std::move(obj2);
    obj5.connect();
    REQUIRE(obj5.is_connected());
}

TEST_CASE("RedisClient object are equal", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    simple_redis::RedisConfig config1;
    simple_redis::RedisConfig config2;
    simple_redis::RedisClient obj1(config1);
    simple_redis::RedisClient obj2(config2);
    REQUIRE(obj1 == obj2);
    simple_redis::RedisClient obj3(std::move(obj1));
    REQUIRE(obj3 == obj2);
    simple_redis::RedisClient obj4 = obj2;
    REQUIRE(obj4 == obj2);
    REQUIRE(obj4 == obj3);
    simple_redis::RedisClient obj5 = std::move(obj2);
    REQUIRE(obj3 == obj5);
    REQUIRE(obj4 == obj5);
}

TEST_CASE("RedisClient command to all nodes", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    // REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS
    simple_redis::RedisClient obj1(global_config);
    obj1.connect();  // fail if connect to a slave
    auto masters = obj1.get_masters_nodes();
    REQUIRE(!masters.empty());
    for (auto &master: masters) {
        auto result = master.ping();
        REQUIRE(result == "PONG");
    }
    auto slaves = obj1.get_slaves_nodes();
    REQUIRE(!slaves.empty());
    for (auto &slave: slaves) {
        auto result = slave.ping();
        REQUIRE(result == "PONG");
    }
}

TEST_CASE("RedisClient command scan to all nodes", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    // REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS
    simple_redis::RedisClient obj1(global_config);
    obj1.connect();  // fail if connect to a slave
    auto masters = obj1.get_masters_nodes();
    REQUIRE(!masters.empty());
    std::vector<std::string> keys;
    long long int cursor = 0;
    for (auto &master: masters) {
        do {
            cursor = master.scan(cursor, "*", std::inserter(keys, keys.begin()));
        } while (cursor != 0);
    }
}


TEST_CASE("RedisClient command scan to all nodes thread", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    // REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS
    simple_redis::RedisClient obj1(global_config);
    obj1.connect();  // fail if connect to a slave
    auto masters = obj1.get_cluster_nodes_master();
    REQUIRE(!masters.empty());
    std::vector<std::string> keys;
    std::vector<std::thread> threads;
    std::mutex result_mutex;
    for (const auto &master: masters) {
        threads.emplace_back([&obj1, &master, &keys, &result_mutex] {
            auto temp_keys = obj1.get_keys_in_node(master);
            std::lock_guard<std::mutex> lock(result_mutex);
            keys.insert(keys.end(), temp_keys.begin(), temp_keys.end());
        });
    }
    for (auto &thread: threads) {
        thread.join();
    }
}

TEST_CASE("RedisClient command scan to all nodes thread pattern", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    // REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS
    simple_redis::RedisClient obj1(global_config);
    obj1.connect();  // fail if connect to a slave
    auto masters = obj1.get_cluster_nodes_master();
    REQUIRE(!masters.empty());
    std::vector<std::string> keys;
    std::vector<std::thread> threads;
    std::mutex result_mutex;

    for (const auto &master: masters) {
        threads.emplace_back([&obj1, &master, &keys, &result_mutex] {
            auto temp_keys = obj1.get_keys_in_node(master, "hol*");
            std::lock_guard<std::mutex> lock(result_mutex);
            keys.insert(keys.end(), temp_keys.begin(), temp_keys.end());
        });
    }
    for (auto &thread: threads) {
        thread.join();
    }
}


TEST_CASE("RedisClient keys", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    // REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS
    simple_redis::RedisClient obj1(global_config);
    obj1.connect();  // fail if connect to a slave
    auto masters = obj1.get_cluster_nodes_master();
    REQUIRE(!masters.empty());
    std::vector<std::string> keys = obj1.keys();
}


TEST_CASE("RedisClient get without connect", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    simple_redis::RedisClient obj1(global_config);
    REQUIRE_FALSE(obj1.check_servers());
}

TEST_CASE("RedisClient cluster nodes", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    // REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS
    simple_redis::RedisClient obj1(global_config);
    obj1.connect();
    SECTION("Master once") {
        auto masters = obj1.get_cluster_nodes_master();
        REQUIRE(!masters.empty());
        for (auto &master: masters) {
            REQUIRE(simple_common::is_ip(master.first));
            REQUIRE(simple_common::is_a_valid_port(master.second));
        }

    }SECTION("Slave one") {
        auto slaves = obj1.get_cluster_nodes_slave();
        REQUIRE(!slaves.empty());
        for (auto &slave: slaves) {
            REQUIRE(simple_common::is_ip(slave.first));
            REQUIRE(simple_common::is_a_valid_port(slave.second));
        }
    }

    SECTION("Master 100 times") {
        for (int i = 0; i < 100; ++i) {
            auto masters = obj1.get_cluster_nodes_master();
            REQUIRE(!masters.empty());
            for (auto &master: masters) {
                REQUIRE(simple_common::is_ip(master.first));
                REQUIRE(simple_common::is_a_valid_port(master.second));
            }
        }
    }SECTION("Slave 100 times") {
        for (int i = 0; i < 100; ++i) {
            auto slaves = obj1.get_cluster_nodes_slave();
            REQUIRE(!slaves.empty());
            for (auto &slave: slaves) {
                REQUIRE(simple_common::is_ip(slave.first));
                REQUIRE(simple_common::is_a_valid_port(slave.second));
            }
        }
    }
}
