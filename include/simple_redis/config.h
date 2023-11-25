//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#ifndef SIMPLE_REDIS_CONFIG_H
#define SIMPLE_REDIS_CONFIG_H

#include <nlohmann/json.hpp>
#include <simple_logger/logger.h>
#include <simple_config/config.h>
#include <sw/redis++/redis++.h>
#include <simple_redis/basic.h>

namespace simple_redis {

    class RedisConfig : public simple_config::Config {
    private:
        static ClusterConnectionOptions m_set_connection_options();
    public:

        [[maybe_unused]] [[nodiscard]] bool validate() override;

        [[nodiscard]] std::string to_string() const override;

        [[nodiscard]] json to_json() const override;

        void from_json(const json &j) override;

        std::shared_ptr<simple_logger::Logger> logger = std::make_shared<simple_logger::Logger>(loglevel);
        std::shared_ptr<ClusterConnectionOptions> connection_options = std::make_shared<ClusterConnectionOptions>(
                m_set_connection_options());
    };
}
#endif //SIMPLE_REDIS_CONFIG_H
