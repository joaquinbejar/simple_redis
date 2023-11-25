//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#ifndef SIMPLE_REDIS_LIFO_H
#define SIMPLE_REDIS_LIFO_H

#include <simple_redis/fifo.h>

namespace simple_redis {

    using json = nlohmann::json;

    class LIFORedisClient : public FIFORedisClient {
    public:
        // Constructor
        explicit LIFORedisClient(RedisConfig &config);

        std::string get(const std::string &key) override;

        std::string get(const std::string &&key) override;

        std::vector<std::string> get(const std::string &key, long size) override;

        std::vector<std::string> get(const std::string &&key, long size) override;

        std::string nb_get(const std::string &key) override;

        std::string nb_get(const std::string &&key) override;

        std::vector<std::string> nb_get(const std::string &key, long size) override;

        std::vector<std::string> nb_get(const std::string &&key, long size) override;
    };
}
#endif //SIMPLE_REDIS_LIFO_H
