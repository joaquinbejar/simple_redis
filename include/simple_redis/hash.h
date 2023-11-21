//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#ifndef CPP_REDIS_BASE_HASH_H
#define CPP_REDIS_BASE_HASH_H

#include <nlohmann/json.hpp>

#include <simple_redis/config.h>
#include <simple_redis/client.h>

namespace simple_redis {

    using json = nlohmann::json;


    class HashRedisClient : public RedisClient {
    public:
        // Constructor
        explicit HashRedisClient(RedisConfig &config);

        bool set(std::string &hash_key, const std::string &key, const std::string &value);

        bool set(std::string &&hash_key, const std::string &key, const std::string &value);

        bool set(std::string &key, std::map<std::string, std::string> &value);

        std::string get(std::string &hash_key, const std::string &key);

        std::map<std::string, std::string> get(std::string &hash_key);

        std::vector<std::string> getkeys(std::string &hash_key);

        bool delkey(std::string &hash_key, const std::string &key);

    };
}
#endif //CPP_REDIS_BASE_HASH_H
