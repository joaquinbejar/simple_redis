//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#include "config.h"

namespace simple_redis {

// ------------------------------------------------------------------------------------------

    RedisConfig::RedisConfig() = default;


    RedisConfig::RedisConfig(const RedisConfig &other) : Config(other) {
        if (this == &other) {
            return;
        }
        this->connection_options = other.connection_options;
        this->logger = simple_logger::Logger(other.logger.getLevel());
    }

    RedisConfig::RedisConfig(RedisConfig &&other) noexcept
            : Config(std::move(other)),
              connection_options(std::move(other.connection_options)),
              logger(std::move(other.logger)) {
        other.m_moved = true;
    }

    RedisConfig::RedisConfig(const json &j) {
        connection_options->from_json(j);
        if (j.contains("loglevel")) {
            this->logger.setLevel(j["loglevel"]);
        }
    }

    RedisConfig &RedisConfig::operator=(const RedisConfig &other) {
        if (this == &other) {
            return *this;
        }
        this->connection_options = other.connection_options;
        this->logger = simple_logger::Logger(other.logger.getLevel());
        return *this;

    }

    RedisConfig &RedisConfig::operator=(RedisConfig &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        this->connection_options = std::move(other.connection_options);
        this->logger = std::move(other.logger);
        other.m_moved = true;
        return *this;
    }

    bool RedisConfig::operator==(const RedisConfig &rhs) const {
        if (*this->connection_options != *rhs.connection_options) {
            return false;
        }
        if (this->logger != rhs.logger) {
            return false;
        }
        return true;
    }

    bool RedisConfig::operator!=(const RedisConfig &rhs) const {
        return !(rhs == *this);
    }

    [[maybe_unused]] [[nodiscard]] bool RedisConfig::validate() {
        if (is_moved())
            return false;
        if (connection_options->tag.empty()) {
            logger.log_error("REDIS_TAG is empty");
            return false;
        }
        if (connection_options->host.empty()) {
            logger.log_error("REDIS_HOSTNAME is empty");
            return false;
        }
        if (connection_options->port == 0 || connection_options->port > 65535) {
            logger.log_error("REDIS_PORT is wrong");
            return false;
        }
        if (connection_options->connect_timeout.count() < 0) {
            logger.log_error("REDIS_CONNECT_TIMEOUT is wrong");
            return false;
        }
        if (connection_options->socket_timeout.count() < 0) {
            logger.log_error("REDIS_SOCKET_TIMEOUT is wrong");
            return false;
        }
        if (connection_options->size < 0) {
            logger.log_error("REDIS_POOL_SIZE is wrong");
            return false;
        }
        if (connection_options->wait_timeout.count() < 0) {
            logger.log_error("REDIS_POOL_WAIT_TIMEOUT is wrong");
            return false;
        }
        if (connection_options->connection_lifetime.count() < 0) {
            logger.log_error("REDIS_POOL_CONNECTION_LIFETIME is wrong");
            return false;
        }
        return true;
    }

    std::string RedisConfig::to_string() const {
            return R"({"RedisConfig":)" + this->get_basic_string() + "}";
    }

    json RedisConfig::to_json() const {
        json j = connection_options->to_json();
        j["loglevel"] = this->logger.getLevel();
        return j;
    }

    void RedisConfig::from_json(const json &j) {
        connection_options->from_json(j);
        if (j.contains("loglevel")) {
            this->logger.setLevel(j["loglevel"]);
        }
    }


    std::shared_ptr<ClusterConnectionOptions> RedisConfig::m_set_connection_options() {
        std::shared_ptr<ClusterConnectionOptions> _connection_options = std::make_shared<ClusterConnectionOptions>();
        _connection_options->port = simple_common::get_env_variable_int("REDIS_PORT", 6379);
        _connection_options->host = get_node_from_string(
                simple_common::get_env_variable_string("REDIS_HOSTNAME", "localhost"));
        _connection_options->password = simple_common::get_env_variable_string("REDIS_PASSWORD", "password");
        _connection_options->connect_timeout = std::chrono::milliseconds(
                simple_common::get_env_variable_int("REDIS_CONNECT_TIMEOUT", 30000));
        _connection_options->socket_timeout = std::chrono::milliseconds(
                simple_common::get_env_variable_int("REDIS_SOCKET_TIMEOUT", 30000));
        _connection_options->keep_alive = simple_common::get_env_variable_bool("REDIS_KEEP_ALIVE", true);
        _connection_options->db = simple_common::get_env_variable_int("REDIS_DB", 0);
        _connection_options->tag = simple_common::get_env_variable_string("REDIS_TAG", "tag");
        _connection_options->wait_timeout = std::chrono::milliseconds(
                simple_common::get_env_variable_int("REDIS_WAIT_TIMEOUT", 30000));
        _connection_options->connection_lifetime = std::chrono::milliseconds(
                simple_common::get_env_variable_int("REDIS_CONNECTION_LIFETIME", 0));
        _connection_options->connection_idle_time = std::chrono::milliseconds(
                simple_common::get_env_variable_int("REDIS_CONNECTION_IDLE_TIME", 0));
        _connection_options->size = simple_common::get_env_variable_int("REDIS_SIZE", 1000);

        return _connection_options;
    }
}