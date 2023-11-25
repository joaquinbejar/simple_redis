//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#include "simple_redis/fifo.h"

namespace simple_redis {

    FIFORedisClient::FIFORedisClient(RedisConfig &config) : RedisClient(config) {
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("FIFORedisClient constructor");
    }

    bool FIFORedisClient::set(const std::string &key, const std::string &value) {
        if (!m_server_write)
            return false;
        try {
            return this->m_server_write->rpush(set_tag(m_config.connection_options->tag, key), value);
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB RPUSH failed: " + std::string(e.what()));
        }
        return false;
    }

    bool FIFORedisClient::set(const std::string &&key, const std::string &value) {
        return this->set(key, value);
    }

    bool FIFORedisClient::set(const std::string &key, std::vector<std::string> &value) {
        if (!m_server_write)
            return false;
        try {
            return this->m_server_write->rpush(set_tag(m_config.connection_options->tag, key), value.begin(),
                                               value.end());
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB RPUSH failed: " + std::string(e.what()));
        }
        return false;
    }

    bool FIFORedisClient::set(const std::string &&key, std::vector<std::string> &value) {
        return this->set(key, value);
    }

    std::string FIFORedisClient::get(const std::string &key) {
        if (!m_server_write)
            return {};
        try {
            // blpop is blocking, so we need to use a timeout and user write server
            sw::redis::OptionalStringPair result = this->m_server_write->blpop(
                    set_tag(m_config.connection_options->tag, key),
                    this->m_config.connection_options->blocking_timeout);
            return result->second;
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB BLPOP failed: " + std::string(e.what()));
        }
        return {};
    }

    std::string FIFORedisClient::get(const std::string &&key) {
        return this->get(key);
    }

    std::vector<std::string> FIFORedisClient::get(const std::string &key, long size) {
        if (!m_server_write)
            return {};
        try {
            std::vector<std::string> v_result;
            v_result.reserve(size);
            for (int i = 0; i < size; i++) {

                sw::redis::OptionalStringPair result = this->m_server_write->blpop(
                        set_tag(m_config.connection_options->tag, key),
                        this->m_config.connection_options->blocking_timeout);
                if (result) {
                    v_result.push_back(result->second);
                }
            }
            return v_result;
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB BLPOP failed: " + std::string(e.what()));
        }
        return {};
    }

    std::vector<std::string> FIFORedisClient::get(const std::string &&key, long size) {
        return this->get(key, size);
    }

    std::string FIFORedisClient::nb_get(const std::string &key) {
        if (!m_server_write)
            return {};
        try {
            sw::redis::OptionalString result = this->m_server_write->lpop(
                    set_tag(m_config.connection_options->tag, key));
            return result ? *result : "";
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB LPOP failed: " + std::string(e.what()));
        }
        return {};
    }

    std::string FIFORedisClient::nb_get(const std::string &&key) {
        return this->nb_get(key);
    }

    std::vector<std::string> FIFORedisClient::nb_get(const std::string &key, long size) {
        if (!m_server_write)
            return {};
        try {
            std::vector<std::string> v_result;
            v_result.reserve(size);
            for (int i = 0; i < size; i++) {
                sw::redis::OptionalString result = this->m_server_write->lpop(
                        set_tag(m_config.connection_options->tag, key));
                if (result) {
                    v_result.push_back(*result);
                }
            }
            return v_result;
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB LPOP failed: " + std::string(e.what()));
        }
        return {};
    }

    std::vector<std::string> FIFORedisClient::nb_get(const std::string &&key, long size) {
        return this->nb_get(key, size);
    }

    long long FIFORedisClient::size(std::string &key) {
        if (!m_server_read)
            return 0;
        try {
            return this->m_server_read->llen(set_tag(m_config.connection_options->tag, key));
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB LLEN failed: " + std::string(e.what()));
        }
        return 0;
    }

    long long FIFORedisClient::size(std::string &&key) {
        return this->size(key);
    }


}