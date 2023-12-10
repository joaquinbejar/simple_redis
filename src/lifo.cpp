//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#include "simple_redis/lifo.h"

namespace simple_redis {

    LIFORedisClient::LIFORedisClient(RedisConfig &config) : FIFORedisClient(config) {
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("LIFORedisClient constructor");
    }

    std::string LIFORedisClient::get(const std::string &key) {
        if (!m_server_write)
            return {};
        try {
            // brpop is blocking, so we need to use a timeout and user write server
            sw::redis::OptionalStringPair result = this->m_server_write->brpop(
                    set_tag(m_config.connection_options->tag, key),
                    this->m_config.connection_options->blocking_timeout);
            return result->second;
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB BRPOP failed: " + std::string(e.what()));
        }
        return {};
    }

    std::string LIFORedisClient::get(const std::string &&key) {
        return this->get(key);
    }

    std::vector<std::string> LIFORedisClient::get(const std::string &key, long size) {
        if (!m_server_write)
            return {};
        try {
            std::vector<std::string> v_result;
            v_result.reserve(size);
            for (int i = 0; i < size; i++) {

                sw::redis::OptionalStringPair result = this->m_server_write->brpop(
                        set_tag(m_config.connection_options->tag, key),
                        this->m_config.connection_options->blocking_timeout);
                if (result) {
                    v_result.push_back(result->second);
                }
            }
            return v_result;
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("RedisDB BRMPOP Timeout: " + std::string(e.what()));
        }
        return {};
    }

    std::vector<std::string> LIFORedisClient::get(const std::string &&key, long size) {
        return this->get(key, size);
    }

    std::string LIFORedisClient::nb_get(const std::string &key) {
        if (!m_server_write)
            return {};
        try {
            sw::redis::OptionalString result = this->m_server_write->rpop(
                    set_tag(m_config.connection_options->tag, key));
            return result ? *result : "";
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB RPOP failed: " + std::string(e.what()));
        }
        return {};
    }

    std::string LIFORedisClient::nb_get(const std::string &&key) {
        return this->nb_get(key);
    }

    std::vector<std::string> LIFORedisClient::nb_get(const std::string &key, long size) {
        if (!m_server_write)
            return {};
        try {
            std::vector<std::string> v_result;
            v_result.reserve(size);
            for (int i = 0; i < size; i++) {

                sw::redis::OptionalString result = this->m_server_write->rpop(
                        set_tag(m_config.connection_options->tag, key));
                if (result) {
                    v_result.push_back(*result);
                }
            }
            return v_result;
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB RPOP failed: " + std::string(e.what()));
        }
        return {};
    }

    std::vector<std::string> LIFORedisClient::nb_get(const std::string &&key, long size) {
        return this->nb_get(key, size);
    }

}