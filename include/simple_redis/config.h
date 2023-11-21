//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#ifndef CPP_REDIS_BASE_CONFIG_H
#define CPP_REDIS_BASE_CONFIG_H

#include <nlohmann/json.hpp>
#include <simple_logger/logger.h>
#include <simple_config/config.h>
#include <sw/redis++/redis++.h>
#include <simple_redis/basic.h>

namespace simple_redis {

    using json = nlohmann::json;

    class RedisConfig : public simple_config::Config {
    public:
        RedisConfig();

        RedisConfig(const RedisConfig &other);

        RedisConfig(RedisConfig &&other) noexcept;

        explicit RedisConfig(const json &j);

        // Copy assignment operator
        RedisConfig &operator=(const RedisConfig &other);

        // Move assignment operator
        RedisConfig &operator=(RedisConfig &&other) noexcept;

        bool operator==(const RedisConfig &rhs) const;

        bool operator!=(const RedisConfig &rhs) const;

        [[maybe_unused]] [[nodiscard]] bool validate() override;

        [[nodiscard]] std::string to_string() const override;

        [[nodiscard]] json to_json() const override;

        void from_json(const json &j) override;


        simple_logger::Logger logger = simple_logger::Logger(this->loglevel);
        std::shared_ptr<ClusterConnectionOptions> connection_options = m_set_connection_options();

    private:

        static std::shared_ptr<ClusterConnectionOptions> m_set_connection_options();

    };
}
#endif //CPP_REDIS_BASE_CONFIG_H
