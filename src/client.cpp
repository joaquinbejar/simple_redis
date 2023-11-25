//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#include "simple_redis/client.h"

namespace simple_redis {

    RedisAccessViolation::RedisAccessViolation(simple_logger::Logger &logger, const std::string &message) {
        logger.send<simple_logger::LogLevel::ERROR>(message);
    }

    RedisClient::RedisClient(RedisConfig &config) : m_config(config) {
        if (this->m_config.validate()) {
            this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("RedisDB client created with valid config");
        } else {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB client created with invalid config");
            throw std::invalid_argument("RedisDB client created with invalid config");
        }
    }

//    RedisClient::~RedisClient() {
//        if (!this->m_config.is_moved()) {
//            this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("RedisDB client destructor");
//        }
//
//    }
//
//    RedisClient::RedisClient(const RedisClient &other) {
//        m_config.logger->send<simple_logger::LogLevel::DEBUG>("RedisDB client copy constructor");
//        if (this == &other) {
//            m_config.logger->send<simple_logger::LogLevel::DEBUG>("This and other are the same");
//            return;
//        }
//        this->m_config = other.m_config;
//    }
//
//    RedisClient::RedisClient(RedisClient &&other) noexcept {
//        if (this == &other) {
//            return;
//        }
//        this->m_config = std::move(other.m_config);
//    }
//
//    RedisClient &RedisClient::operator=(const RedisClient &other) {
//        if (this == &other) {
//            return *this;
//        }
//        this->m_config = other.m_config;
//        return *this;
//    }
//
//    RedisClient &RedisClient::operator=(RedisClient &&other) noexcept {
//        if (this == &other) {
//            return *this;
//        }
//        this->m_config = std::move(other.m_config);
//        return *this;
//    }
//
//    bool RedisClient::operator==(const RedisClient &rhs) const {
//        if (this->m_config != rhs.m_config) {
//            return false;
//        }
//        return true;
//    }
//
//    bool RedisClient::operator!=(const RedisClient &rhs) const {
//        return !(rhs == *this);
//    }

    void RedisClient::connect() {
        if (!this->m_config.validate()) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                    "RedisDB trying to connect with invalid config");
            return;
        }
        json json_config = this->m_config.to_json();
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("Connecting to RedisDB");
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>(
                "RedisDB password: " + json_config["password"].get<std::string>());
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>(
                "RedisDB hostname: " + json_config["host"].get<std::string>());
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>(
                "RedisDB port: " + std::to_string(json_config["port"].get<int>()));
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>(
                "RedisDB tag: " + json_config["tag"].get<std::string>());
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>(
                "RedisDB size: " + std::to_string(json_config["size"].get<int>()));
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("RedisDB connect_timeout: " +
                                                                    std::to_string(
                                                                            json_config["connect_timeout"].get<int>()));
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("RedisDB socket_timeout: " +
                                                                    std::to_string(
                                                                            json_config["socket_timeout"].get<int>()));
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("RedisDB loglevel: " + *this->m_config.loglevel);
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>(
                "RedisDB keep_alive: " + std::to_string(json_config["keep_alive"].get<bool>()));
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>(
                "RedisDB db: " + std::to_string(json_config["db"].get<int>()));
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("RedisDB wait_timeout: " +
                                                                    std::to_string(
                                                                            json_config["wait_timeout"].get<int>()));
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("RedisDB connection_lifetime: " +
                                                                    std::to_string(
                                                                            json_config["connection_lifetime"].get<int>()));
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>("RedisDB connection_idle_time: " +
                                                                    std::to_string(
                                                                            json_config["connection_idle_time"].get<int>()));
        this->m_config.logger->send<simple_logger::LogLevel::DEBUG>(
                "RedisDB Connection size: " + std::to_string(json_config["size"].get<int>()));


        try {
            this->m_server_write = std::make_shared<sw::redis::RedisCluster>(
                    *this->m_config.connection_options,
                    *this->m_config.connection_options
            );
            this->m_server_read = std::make_shared<sw::redis::RedisCluster>(
                    *this->m_config.connection_options,
                    *this->m_config.connection_options, sw::redis::Role::SLAVE
            );

        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                    "RedisDB connection failed to " + this->m_config.connection_options->host + ":" +
                    std::to_string(this->m_config.connection_options->port));
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                    "RedisDB connection failed: " + std::string(e.what()));
            throw e;
        }

        if (this->is_connected()) {
            this->m_config.logger->send<simple_logger::LogLevel::DEBUG>(
                    "RedisDB connected to " + this->m_config.connection_options->host + ":" +
                    std::to_string(this->m_config.connection_options->port));
        }
    }

    bool RedisClient::is_connected() {
        if (m_server_write == nullptr) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("Write server is null");
            return false;
        } else if (m_server_read == nullptr) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("Read server is null");
            return false;
        } else {
            try {
                auto ping_write = m_server_write->redis("ping", false).ping();
                auto ping_read = m_server_read->redis("ping", false).ping();
                return strstr(ping_write.c_str(), m_pong) && strstr(ping_read.c_str(), m_pong);
            } catch (const std::exception &e) {
                this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                        "RedisDB PING failed: " + std::string(e.what()));
                return false;
            }
        }
    }

    bool RedisClient::del(const std::string &key) {
        if (!m_server_write) {
            return false;
        }
        try {
            return this->m_server_write->del(set_tag(m_config.connection_options->tag, key));
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB DEL failed: " + std::string(e.what()));
        }
        return false;
    }

    bool RedisClient::del(const std::vector<std::string> &keys) {
        if (!m_server_write)
            return false;
        try {
            std::vector<std::string> _keys = set_tag(m_config.connection_options->tag, keys);
            return this->m_server_write->del(_keys.begin(), _keys.end());
        } catch (const sw::redis::ReplyError &rep_e) {
            this->m_config.logger->send<simple_logger::LogLevel::INFORMATIONAL>(
                    "RedisDB sw::redis::ReplyError DEL failed: " + std::string(rep_e.what()));
            std::vector<bool> result_delete;
            result_delete.reserve(keys.size());
            for (auto &key: keys) {
                result_delete.push_back(this->del(key));
            }
            return std::all_of(result_delete.begin(), result_delete.end(), [](bool v) { return v; });
        } catch (const sw::redis::Error &r_e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                    "RedisDB sw::redis::Error DEL failed: " + std::string(r_e.what()) + " exception: " +
                    typeid(r_e).name());
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                    "RedisDB std::exception DEL failed: " + std::string(e.what()));
        }
        return false;
    }

    bool RedisClient::del(const std::string &&key) {
        if (!m_server_write)
            return false;
        try {
            return this->m_server_write->del(set_tag(m_config.connection_options->tag, key));
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>("RedisDB DEL failed: " + std::string(e.what()));
        }
        return false;
    }

    bool RedisClient::del(const std::set<std::string> &keys) {
        std::vector<std::string> _keys;
        _keys.reserve(keys.size());
        for (auto &key: keys) {
            _keys.push_back(key);
        }
        return this->del(_keys);
    }

    unsigned long long RedisClient::size() {
        if (!m_server_read)
            return 0;
        return m_server_read->redis("dbsize", false).dbsize();
    }

    std::vector<std::pair<std::string, std::string>> RedisClient::get_cluster_nodes_master() {
        std::vector<std::pair<std::string, std::string>> result;
        try {
            if (!m_server_read)
                return {};
            auto cluster_nodes = m_server_read->command("cluster", "nodes");

            std::string line;
            std::istringstream f(cluster_nodes->str);
            while (std::getline(f, line)) {
                if (line.find("master") != std::string::npos) {
                    auto ip_port = common::ip::ip_and_port_from_string_to_pair(line);
                    result.push_back(ip_port);
                }
            }
            return result;
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                    "RedisDB get_cluster_nodes_master failed: " + std::string(e.what()));
        }
        return result;
    }

    std::vector<std::pair<std::string, std::string>> RedisClient::get_cluster_nodes_slave() {
        std::vector<std::pair<std::string, std::string>> result = {};
        try {
            if (!m_server_read)
                return result;
            auto cluster_nodes = m_server_read->command("cluster", "nodes");
            std::string line;
            std::istringstream f(cluster_nodes->str);
            while (std::getline(f, line)) {
                if (line.find("slave") != std::string::npos) {
                    auto ip_port = common::ip::ip_and_port_from_string_to_pair(line);
                    result.push_back(ip_port);
                }
            }
            return result;
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                    "RedisDB get_cluster_nodes_slave failed: " + std::string(e.what()));
        }
        return result;
    }

    std::vector<sw::redis::Redis> RedisClient::get_masters_nodes() {
        auto masters = get_cluster_nodes_master();
        std::vector<sw::redis::Redis> result;
        for (auto &master: masters) {
            sw::redis::ConnectionOptions connection_options = *m_config.connection_options;
            connection_options.host = master.first;
            connection_options.port = std::stoi(master.second);
            if (common::ip::is_ip(connection_options.host) &&
                common::ip::is_a_valid_port(connection_options.port))
                result.emplace_back(connection_options);
        }
        return result;
    }

    std::vector<sw::redis::Redis> RedisClient::get_slaves_nodes() {
        auto slaves = get_cluster_nodes_slave();
        std::vector<sw::redis::Redis> result;
        for (auto &master: slaves) {
            sw::redis::ConnectionOptions connection_options = *m_config.connection_options;
            connection_options.host = master.first;
            connection_options.port = std::stoi(master.second);
            if (common::ip::is_ip(connection_options.host) &&
                common::ip::is_a_valid_port(connection_options.port))
                result.emplace_back(connection_options);
        }
        return result;
    }

    std::vector<std::string> RedisClient::get_keys_in_node(const std::pair<std::string, std::string> &node) {
        return get_keys_in_node(node, "*");
    }

    std::vector<std::string>
    RedisClient::get_keys_in_node(const std::pair<std::string, std::string> &node, const std::string &pattern) {
        std::vector<std::string> keys;
        try {
            sw::redis::ConnectionOptions connection_options;
            connection_options = *m_config.connection_options;
            if (node.first.empty() || node.second.empty())
                return keys;
            connection_options.host = node.first;
            connection_options.port = std::stoi(node.second);
            sw::redis::Redis redis(connection_options);
            try {
                long long int cursor = 0;
                do {
                    cursor = redis.scan(cursor, pattern, std::inserter(keys, keys.begin()));
                } while (cursor != 0);
                return keys;
            } catch (const std::exception &e) {
                this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                        "RedisDB std::exception GET_KEYS_IN_NODE SCAN failed: " + std::string(e.what()));
            }
        } catch (const sw::redis::Error &r_e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                    "RedisDB sw::redis::Error GET_KEYS_IN_NODE failed: " + std::string(r_e.what()) + " exception: " +
                    typeid(r_e).name());
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                    "Error in node: " + node.first + ":" + node.second);

        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                    "RedisDB std::exception GET_KEYS_IN_NODE failed: " + std::string(e.what()));
        }
        return keys;
    }

    std::vector<std::string> RedisClient::keys() {
        return keys("*");
    }

    std::vector<std::string> RedisClient::keys(const std::string &pattern) {
        std::vector<std::string> keys = {};
        try {
            std::vector<std::future<void>> futures;
            for (const auto &slave: this->get_cluster_nodes_slave()) {
                futures.emplace_back(std::async(std::launch::async, [slave, pattern, &keys, this] {
                    auto temp_keys = this->get_keys_in_node(slave, pattern);
                    std::lock_guard<std::mutex> lock(m_result_mutex_keys);
                    keys.insert(keys.end(), temp_keys.begin(), temp_keys.end());
                }));
            }
            for (auto &future: futures) {
                future.wait();
            }
            return keys;
        } catch (const std::exception &e) {
            this->m_config.logger->send<simple_logger::LogLevel::ERROR>(
                    "RedisDB std::exception KEYS failed: " + std::string(e.what()));
        }
        return keys;
    }

    [[maybe_unused]] bool RedisClient::check_servers() {
        try {
            if (!m_server_read || !m_server_write) {
                return false;
            }
        } catch (const std::exception &e) {
            throw RedisAccessViolation(*this->m_config.logger,
                                       "RedisDB check_servers failed: " + std::string(e.what()));
        }
        return true;
    }


}