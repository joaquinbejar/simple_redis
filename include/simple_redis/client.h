//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#ifndef CPP_REDIS_BASE_CLIENT_H
#define CPP_REDIS_BASE_CLIENT_H

#include <hiredis/hiredis.h>
#include <hiredis.h>
#include <nlohmann/json.hpp>
#include <sw/redis++/redis++.h>
#include <simple_redis/config.h>
#include <set>
#include <common/common.h>
#include <common/ip.h>
#include <future>


namespace simple_redis {

    using json = nlohmann::json;

    class RedisAccessViolation : public std::exception {
    public:
        explicit RedisAccessViolation(const std::string &message);

    private:
        std::string m_message;
    };

    class RedisClient {
    public:
        // Constructor
        explicit RedisClient(RedisConfig &config);

//        // Destructor
//        ~RedisClient();
//
//        // Copy constructor
//        RedisClient(const RedisClient &other);
//
//        // Move constructor
//        RedisClient(RedisClient &&other) noexcept;
//
//        // Copy assignment operator
//        RedisClient &operator=(const RedisClient &other);
//
//        // Move assignment operator
//        RedisClient &operator=(RedisClient &&other) noexcept;
//
//        // Equality operator
//        bool operator==(const RedisClient &rhs) const;
//
//        // Inequality operator
//        bool operator!=(const RedisClient &rhs) const;

        bool is_connected();

        void connect();

        bool del(const std::string &key);

        bool del(const std::string &&key);

        bool del(const std::vector<std::string> &keys);

        bool del(const std::set<std::string> &keys);

        virtual unsigned long long size();

        std::vector<std::pair<std::string, std::string>> get_cluster_nodes_master();

        std::vector<std::pair<std::string, std::string>> get_cluster_nodes_slave();

        std::vector<sw::redis::Redis> get_masters_nodes();

        std::vector<sw::redis::Redis> get_slaves_nodes();

        std::vector<std::string> get_keys_in_node(const std::pair<std::string, std::string> &node);

        std::vector<std::string>
        get_keys_in_node(const std::pair<std::string, std::string> &node, const std::string &pattern);

        std::vector<std::string> keys();

        std::vector<std::string> keys(const std::string &pattern);

        [[maybe_unused]] bool check_servers();


    private:
        const char *m_pong = "PONG";
        std::mutex m_result_mutex_keys;


    protected:
        std::shared_ptr<sw::redis::RedisCluster> m_server_write;
        std::shared_ptr<sw::redis::RedisCluster> m_server_read;
        RedisConfig m_config;


    };

}
#endif //CPP_REDIS_BASE_CLIENT_H
