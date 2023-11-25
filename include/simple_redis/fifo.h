//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#ifndef SIMPLE_REDIS_FIFO_H
#define SIMPLE_REDIS_FIFO_H

#include <simple_redis/client.h>

namespace simple_redis {

    using json = nlohmann::json;

    class FIFORedisClient : public RedisClient {
    public:
        // Constructor
        explicit FIFORedisClient(RedisConfig &config);

        bool set(const std::string &key, const std::string &value);

        bool set(const std::string &&key, const std::string &value);

        bool set(const std::string &key, std::vector<std::string> &value);

        bool set(const std::string &&key, std::vector<std::string> &value);

        virtual std::string get(const std::string &key);

        virtual std::string get(const std::string &&key);

        virtual std::vector<std::string> get(const std::string &key, long size);

        virtual std::vector<std::string> get(const std::string &&key, long size);

        virtual std::string nb_get(const std::string &key);

        virtual std::string nb_get(const std::string &&key);

        virtual std::vector<std::string> nb_get(const std::string &key, long size);

        virtual std::vector<std::string> nb_get(const std::string &&key, long size);

        long long size(std::string &key);

        long long size(std::string &&key);

    };
}
#endif //SIMPLE_REDIS_FIFO_H
