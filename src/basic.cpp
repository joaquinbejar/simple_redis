//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#include "basic.h"

namespace simple_redis {
    std::string get_node_from_string(const std::string &nodes, char delimiter) {
        if (nodes.empty()) {
            return "localhost";
        }
        std::vector<std::string> result;
        std::stringstream ss(nodes);
        std::string item;
        while (std::getline(ss, item, delimiter)) {
            result.push_back(item);
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> dist(0, result.size() - 1);
        std::size_t randomIndex = dist(gen);

        return result[randomIndex];
    }

    std::string set_tag(const std::string &tag, const std::string &key) {
        if (tag.empty()) {
            return "__empty_tag__:" + key;
        }
        return "__" + tag + "__:" + key;
    }

    std::string set_tag(const std::string &tag, const std::string &&key) {
        return set_tag(tag, key);
    }

    std::vector<std::string> set_tag(const std::string &tag, const std::vector<std::string> &key) {
        std::vector<std::string> result;
        result.reserve(key.size());
        for (auto &i: key) {
            if (tag.empty()) {
                result.emplace_back("__empty_tag__:" + i);
            } else {
                result.push_back(set_tag(tag, i));
            }
        }
        return result;
    }


    std::string del_tag(const std::string &key) {
        try {
            std::string tempkey = key;
            size_t pos = key.find(':');
            if (pos != std::string::npos) {
                tempkey.erase(0, pos + 1);
            }
            return tempkey;
        } catch (std::exception &e) {
            std::cerr << "Error on del_tag: " << e.what() << std::endl;
        }
        return "";
    }

    std::string del_tag(const std::string &&key) {
        return del_tag(key);
    }

    void del_tag(std::vector<std::string> &keys) {
        try {
            std::vector<std::string> t_keys = keys;
            keys.clear();
            for (auto &key: t_keys) {
                keys.emplace_back(del_tag(key));
            }
        } catch (std::exception &e) {
            std::cerr << "Error on del_tag: " << e.what() << std::endl;
        }
    }

// ------------------------------------------------------------------------------------------

    std::string ClusterConnectionOptions::to_string() const {
        return this->to_json().dump();
    }

    json ClusterConnectionOptions::to_json() const {
        json j;
        j["host"] = host;
        j["port"] = port;
        j["password"] = password;
        j["db"] = db;
        j["connect_timeout"] = connect_timeout.count();
        j["socket_timeout"] = socket_timeout.count();
        j["keep_alive"] = keep_alive;
        j["size"] = size;
        j["wait_timeout"] = wait_timeout.count();
        j["connection_lifetime"] = connection_lifetime.count();
        j["connection_idle_time"] = connection_idle_time.count();
        j["tag"] = tag;
        return j;
    }

    void ClusterConnectionOptions::from_json(const json &j) {
        try {
            host = j.at("host").get<std::string>();
            port = j.at("port").get<int>();
            password = j.at("password").get<std::string>();
            db = j.at("db").get<int>();
            connect_timeout = std::chrono::milliseconds(j.at("connect_timeout").get<int>());
            socket_timeout = std::chrono::milliseconds(j.at("socket_timeout").get<int>());
            keep_alive = j.at("keep_alive").get<bool>();
            size = j.at("size").get<int>();
            wait_timeout = std::chrono::milliseconds(j.at("wait_timeout").get<int>());
            connection_lifetime = std::chrono::milliseconds(j.at("connection_lifetime").get<int>());
            connection_idle_time = std::chrono::milliseconds(j.at("connection_idle_time").get<int>());
            tag = j.at("tag").get<std::string>();
        } catch (std::exception &e) {
            throw std::runtime_error("Error parsing ClusterConnectionOptions: " + std::string(e.what()));
        }

    }

    bool ClusterConnectionOptions::operator==(const ClusterConnectionOptions &rhs) const {
        if (host != rhs.host) {
            return false;
        }
        if (port != rhs.port) {
            return false;
        }
        if (password != rhs.password) {
            return false;
        }
        if (db != rhs.db) {
            return false;
        }
        if (connect_timeout != rhs.connect_timeout) {
            return false;
        }
        if (socket_timeout != rhs.socket_timeout) {
            return false;
        }
        if (keep_alive != rhs.keep_alive) {
            return false;
        }
        if (size != rhs.size) {
            return false;
        }
        if (wait_timeout != rhs.wait_timeout) {
            return false;
        }
        if (connection_lifetime != rhs.connection_lifetime) {
            return false;
        }
        if (connection_idle_time != rhs.connection_idle_time) {
            return false;
        }
        if (tag != rhs.tag) {
            return false;
        }
        return true;
    }

    bool ClusterConnectionOptions::operator!=(const ClusterConnectionOptions &rhs) const {
        return !(rhs == *this);
    }
}