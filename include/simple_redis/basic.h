//
// Created by Joaquin Bejar Garcia on 2/3/23.
//

#ifndef CPP_REDIS_BASE_BASIC_H
#define CPP_REDIS_BASE_BASIC_H

#include <nlohmann/json.hpp>
#include <sw/redis++/redis++.h>
#include <iostream>

namespace simple_redis {

    using json = nlohmann::json;

    std::string get_node_from_string(const std::string &nodes, char delimiter = ',');

    std::string set_tag(const std::string &tag, const std::string &key);

    std::string set_tag(const std::string &tag, const std::string &&key);

    std::vector<std::string> set_tag(const std::string &tag, const std::vector<std::string> &key);

    std::string del_tag(const std::string &key);

    std::string del_tag(const std::string &&key);

    void del_tag(std::vector<std::string> &keys);


    class ClusterConnectionOptions : public sw::redis::ConnectionOptions, public sw::redis::ConnectionPoolOptions {
    public:
        [[nodiscard]] std::string to_string() const;

        [[nodiscard]] json to_json() const;

        void from_json(const json &j);

        std::string tag = "tag";

        long long blocking_timeout = 30;

        bool operator==(const ClusterConnectionOptions &rhs) const;

        bool operator!=(const ClusterConnectionOptions &rhs) const;
    };
}
#endif //CPP_REDIS_BASE_BASIC_H
