//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#include "simple_redis/set.h"

namespace simple_redis {

    SetRedisClient::SetRedisClient(RedisConfig &config) : RedisClient(config) {
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("SetRedisClient constructor");
    }

    bool SetRedisClient::set(const std::string &key, const std::string &value, unsigned long long ttl) {
        if (!m_server_write)
            return false;
        try {
            if (ttl == 0) {
                return this->m_server_write->set(set_tag(m_config.connection_options->tag, key), value);
            } else {
                ttl *= 1000; // convert to milliseconds
                return this->m_server_write->set(set_tag(m_config.connection_options->tag, key), value,
                                                 std::chrono::milliseconds(ttl));
            }
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB SET failed: " + std::string(e.what()));
        }
        return false;
    }

    bool SetRedisClient::set(const std::string &&key, const std::string &value, unsigned long long ttl) {
        return this->set(key, value, ttl);
    }

    bool SetRedisClient::set(const std::map<std::string, std::string> &value, unsigned long long ttl) {
        try {
            std::vector<bool> v_result = {};
            v_result.reserve(value.size());
            for (std::pair<std::string, std::string> item: value) {
                v_result.push_back(this->set(item.first, item.second, ttl));
            }
            return std::all_of(v_result.begin(), v_result.end(), [](bool v) { return v; });
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB SET failed: " + std::string(e.what()));
        }
        return false;
    }

    bool SetRedisClient::set(const std::map<std::string, std::string> &&value, unsigned long long ttl) {
        return this->set(value, ttl);
    }

    std::string SetRedisClient::get(const std::string &key, const std::string &default_value) {
        if (!m_server_read)
            return {};
        try {
            sw::redis::OptionalString result = this->m_server_read->get(set_tag(m_config.connection_options->tag, key));
            return result.value_or(default_value);
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB GET failed: " + std::string(e.what()));
        }
        return {};
    }

    std::string SetRedisClient::get(const std::string &&key, const std::string &default_value) {
        return this->get(key, default_value);
    }

    std::map<std::string, std::string>
    SetRedisClient::get(const std::vector<std::string> &keys, const std::string &default_value) {
        std::map<std::string, std::string> m_result;
        for (const std::string& key: keys) {
            m_result[key] = this->get(key, default_value);
        }
        return m_result;
    }

    std::map<std::string, std::string>
    SetRedisClient::get(const std::vector<std::string> &&keys, const std::string &default_value) {
        return this->get(keys, default_value);
    }

    std::vector<std::string> SetRedisClient::get_keys() {
        std::vector<std::string> keys = {};
        try {
            const std::string pattern = set_tag(m_config.connection_options->tag, "*");
            keys = this->keys(pattern);
            simple_redis::del_tag(keys);
            return keys;
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB get_keys failed: " + std::string(e.what()));
        }
        return keys;
    }

    bool SetRedisClient::clear() {
        try {
            std::vector<bool> v_result;
            for (const auto& key: this->get_keys()) {
                v_result.push_back(this->del(key));
            }
            return std::all_of(v_result.begin(), v_result.end(), [](bool v) { return v; });
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB clear failed: " + std::string(e.what()));
            return false;
        }
    }

    unsigned long long SetRedisClient::size() {
        return this->get_keys().size();
    }

}