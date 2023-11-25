//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#ifndef SIMPLE_REDIS_SET_H
#define SIMPLE_REDIS_SET_H

#include <simple_redis/client.h>



namespace simple_redis {

    using json = nlohmann::json;

    class SetRedisClient : public RedisClient {
    public:
        // Constructor
        explicit SetRedisClient(RedisConfig &config);

        bool set(const std::string &key, const std::string &value, unsigned long long ttl = 0);

        bool set(const std::string &&key, const std::string &value, unsigned long long ttl = 0);

        bool set(const std::map<std::string, std::string> &value, unsigned long long ttl = 0);

        bool set(const std::map<std::string, std::string> &&value, unsigned long long ttl = 0);

        std::string get(const std::string &key, const std::string &default_value = "");

        std::string get(const std::string &&key, const std::string &default_value = "");

        std::map<std::string, std::string> get(const std::vector<std::string> &keys, const std::string &default_value = "");

        std::map<std::string, std::string> get(const std::vector<std::string> &&keys, const std::string &default_value = "");

        std::vector<std::string> get_keys();

        bool clear();

        unsigned long long size() override;

    };
}
#endif //SIMPLE_REDIS_SET_H
