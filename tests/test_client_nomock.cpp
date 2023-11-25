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

//struct RedisEnvFixtureHostname {
//    RedisEnvFixtureHostname() {
//        setenv("REDIS_HOSTNAME", "other", 1);
//    }
//
//    ~RedisEnvFixtureHostname() {
//        unsetenv("REDIS_HOSTNAME");
//    }
//};
//


TEST_CASE("RedisClient rule of 5 connect", "[RedisDB]") {
    unsetenv("LOGLEVEL");
    // REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS
    simple_redis::RedisClient obj1(global_config);
    obj1.connect();  // fail if connect to a slave
    REQUIRE(obj1.is_connected());
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
    threads.reserve(masters.size());
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
    threads.reserve(masters.size());
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
    // REQUIRE a proper definition in ENV VAR for the connection  DECLARE JUST MASTER AS HOSTS
    simple_redis::RedisClient obj1(global_config);
    obj1.connect();  // fail if connect to a slave
    auto masters = obj1.get_cluster_nodes_master();
    REQUIRE(!masters.empty());
    std::vector<std::string> keys = obj1.keys();
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
            REQUIRE(common::ip::is_ip(master.first));
            REQUIRE(common::ip::is_a_valid_port(master.second));
        }

    }SECTION("Slave one") {
        auto slaves = obj1.get_cluster_nodes_slave();
        REQUIRE(!slaves.empty());
        for (auto &slave: slaves) {
            REQUIRE(common::ip::is_ip(slave.first));
            REQUIRE(common::ip::is_a_valid_port(slave.second));
        }
    }

    SECTION("Master 100 times") {
        for (int i = 0; i < 100; ++i) {
            auto masters = obj1.get_cluster_nodes_master();
            REQUIRE(!masters.empty());
            for (auto &master: masters) {
                REQUIRE(common::ip::is_ip(master.first));
                REQUIRE(common::ip::is_a_valid_port(master.second));
            }
        }
    }SECTION("Slave 100 times") {
        for (int i = 0; i < 100; ++i) {
            auto slaves = obj1.get_cluster_nodes_slave();
            REQUIRE(!slaves.empty());
            for (auto &slave: slaves) {
                REQUIRE(common::ip::is_ip(slave.first));
                REQUIRE(common::ip::is_a_valid_port(slave.second));
            }
        }
    }
}
