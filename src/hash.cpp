//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#include "simple_redis/hash.h"

namespace simple_redis {


    HashRedisClient::HashRedisClient(RedisConfig &config) : RedisClient(config) {
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("HashRedisClient constructor");
    }

    bool HashRedisClient::set(std::string &key, std::map<std::string, std::string> &value) {
        if (!m_server_write)
            return false;
        try {
            return this->m_server_write->hset(set_tag(m_config.connection_options->tag, key), value.begin(),
                                              value.end());
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB HSET failed: " + std::string(e.what()));
        }
        return false;
    }

    bool HashRedisClient::set(std::string &hash_key, const std::string &key, const std::string &value) {
        if (!m_server_write)
            return false;
        try {
            return this->m_server_write->hset(set_tag(m_config.connection_options->tag, hash_key), key, value);
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB HSET failed: " + std::string(e.what()));
        }
        return false;
    }

    bool HashRedisClient::set(std::string &&hash_key, const std::string &key, const std::string &value) {
        return this->set(hash_key, key, value);
    }

    std::string HashRedisClient::get(std::string &hash_key, const std::string &key) {
        if (!m_server_read)
            return {};
        try {
            auto result = this->m_server_read->hget(set_tag(m_config.connection_options->tag, hash_key), key);
            return result.value_or(std::string());
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB HGET failed: " + std::string(e.what()));
        }
        return {};
    }

    std::map<std::string, std::string> HashRedisClient::get(std::string &hash_key) {
        if (!m_server_read)
            return {};
        try {
            std::map<std::string, std::string> result;
            this->m_server_read->hgetall(set_tag(m_config.connection_options->tag, hash_key),
                                         std::inserter(result, result.end()));
            return result;
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB HGETALL failed: " + std::string(e.what()));
        }
        return {};
    }

    std::vector<std::string> HashRedisClient::getkeys(std::string &hash_key) {
        if (!m_server_read)
            return {};
        try {
            std::vector<std::string> result;
            this->m_server_read->hkeys(set_tag(m_config.connection_options->tag, hash_key), std::back_inserter(result));
            return result;
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB HKEYS failed: " + std::string(e.what()));
        }
        return {};
    }

    bool HashRedisClient::delkey(std::string &hash_key, const std::string &key) {
        if (!m_server_write)
            return false;
        try {
            return this->m_server_write->hdel(set_tag(m_config.connection_options->tag, hash_key), key);
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB HDEL failed: " + std::string(e.what()));
        }
        return false;
    }
}