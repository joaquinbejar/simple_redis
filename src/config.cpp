//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#include "simple_redis/config.h"

namespace simple_redis {

    [[maybe_unused]] [[nodiscard]] bool RedisConfig::validate() {
        if (is_moved())
            return false;
        if (connection_options->tag.empty()) {
            logger->send<simple_logger::LogLevel::ERROR>("REDIS_TAG is empty");
            return false;
        }
        if (connection_options->host.empty()) {
            logger->send<simple_logger::LogLevel::ERROR>("REDIS_HOSTNAME is empty");
            return false;
        }
        if (connection_options->port == 0 || connection_options->port > 65535) {
            logger->send<simple_logger::LogLevel::ERROR>("REDIS_PORT is wrong");
            return false;
        }
        if (connection_options->connect_timeout.count() < 0) {
            logger->send<simple_logger::LogLevel::ERROR>("REDIS_CONNECT_TIMEOUT is wrong");
            return false;
        }
        if (connection_options->socket_timeout.count() < 0) {
            logger->send<simple_logger::LogLevel::ERROR>("REDIS_SOCKET_TIMEOUT is wrong");
            return false;
        }
        if (connection_options->size < 0) {
            logger->send<simple_logger::LogLevel::ERROR>("REDIS_SIZE is wrong");
            return false;
        }
        if (connection_options->wait_timeout.count() < 0) {
            logger->send<simple_logger::LogLevel::ERROR>("REDIS_WAIT_TIMEOUT is wrong");
            return false;
        }
        if (connection_options->connection_lifetime.count() < 0) {
            logger->send<simple_logger::LogLevel::ERROR>("REDIS_CONNECTION_LIFETIME is wrong");
            return false;
        }
        return true;
    }

    std::string RedisConfig::to_string() const {
            return R"({"RedisConfig":)" + this->get_basic_string() + "}";
    }

    json RedisConfig::to_json() const {
        json j = connection_options->to_json();
        j["loglevel"] = simple_logger::log_level_to_string(this->logger->getLevel());
        return j;
    }

    void RedisConfig::from_json(const json &j) {
        connection_options->from_json(j);
        if (j.contains("loglevel")) {
            this->logger->setLevel(j["loglevel"].get<std::string>());
        }
    }


    ClusterConnectionOptions RedisConfig::m_set_connection_options() {
        ClusterConnectionOptions _connection_options;
        _connection_options.port = common::get_env_variable_int("REDIS_PORT", 6379);
        _connection_options.host = get_node_from_string(
                common::get_env_variable_string("REDIS_HOSTNAME", "localhost"));
        _connection_options.password = common::get_env_variable_string("REDIS_PASSWORD", "password");
        _connection_options.connect_timeout = std::chrono::milliseconds(
                common::get_env_variable_int("REDIS_CONNECT_TIMEOUT", 30000));
        _connection_options.socket_timeout = std::chrono::milliseconds(
                common::get_env_variable_int("REDIS_SOCKET_TIMEOUT", 30000));
        _connection_options.keep_alive = common::get_env_variable_bool("REDIS_KEEP_ALIVE", true);
        _connection_options.db = common::get_env_variable_int("REDIS_DB", 0);
        _connection_options.tag = common::get_env_variable_string("REDIS_TAG", "tag");
        _connection_options.wait_timeout = std::chrono::milliseconds(
                common::get_env_variable_int("REDIS_WAIT_TIMEOUT", 30000));
        _connection_options.connection_lifetime = std::chrono::milliseconds(
                common::get_env_variable_int("REDIS_CONNECTION_LIFETIME", 0));
        _connection_options.connection_idle_time = std::chrono::milliseconds(
                common::get_env_variable_int("REDIS_CONNECTION_IDLE_TIME", 0));
        _connection_options.size = common::get_env_variable_int("REDIS_SIZE", 1000);

        return _connection_options;
    }
}