//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#ifndef SIMPLE_REDIS_CLIENT_H
#define SIMPLE_REDIS_CLIENT_H

#include <hiredis/hiredis.h>
#include <hiredis.h>
#include <sw/redis++/redis++.h>
#include <simple_redis/config.h>
#include <set>
#include <common/common.h>
#include <common/ip.h>
#include <future>


namespace simple_redis {

    class RedisAccessViolation : public std::exception {
    public:
        explicit RedisAccessViolation( simple_logger::Logger &logger, const std::string &message);
    };

    class RedisClient {
    public:
        // Constructor
        explicit RedisClient(RedisConfig &config);

        // Destructor
        ~RedisClient() = default;

        // Copy constructor
        RedisClient(const RedisClient &other) = delete;

        // Move constructor
        RedisClient(RedisClient &&other) noexcept = delete;

        // Copy assignment operator
        RedisClient &operator=(const RedisClient &other) = delete;

        // Move assignment operator
        RedisClient &operator=(RedisClient &&other) noexcept = delete;

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

        std::vector<std::string> get_keys_in_node(const std::pair<std::string, std::string> &node) const;

        std::vector<std::string>
        get_keys_in_node(const std::pair<std::string, std::string> &node, const std::string &pattern) const;

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
#endif //SIMPLE_REDIS_CLIENT_H
