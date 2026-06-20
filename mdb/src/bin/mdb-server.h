#include <cstdint>
#include <iostream>
#include <thread>

#include "bin/common.h"
#include "graph_models/exceptions.h"
#include "graph_models/gql/gql_model.h"
#include "graph_models/quad_model/quad_model.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "misc/fatal_error.h"
#include "network/server/protocol.h"
#include "network/server/server.h"
#include "query/parser/paths/regular_path_expr.h"
#include "system/buffer_manager.h"
#include "system/string_manager.h"
#include "system/system.h"

namespace MdbBin {

struct SystemConfig {
    bool no_browser = false;

    uint_fast32_t port = MDBServer::Protocol::DEFAULT_PORT;
    uint_fast32_t browser_port = MDBServer::Protocol::DEFAULT_BROWSER_PORT;
    uint_fast32_t workers = std::thread::hardware_concurrency();

    uint64_t limit = 0; // 0 means no limit
    uint64_t strings_static_buffer = StringManager::DEFAULT_STATIC_BUFFER;
    uint64_t strings_dynamic_buffer = StringManager::DEFAULT_DYNAMIC_BUFFER;
    uint64_t private_pages_buffer = BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE;
    uint64_t versioned_pages_buffer = BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE;
    uint64_t unversioned_pages_buffer = BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE;
    uint64_t tensor_store_buffer = TensorStoreManager::DEFAULT_TENSOR_BUFFER_SIZE;

    PathSearchMode path_mode = PathSearchMode::BFS;

    std::chrono::seconds query_timeout = MDBServer::Protocol::DEFAULT_QUERY_TIMEOUT_SECONDS;

    std::string db_directory;

    std::string admin_user;
    std::string admin_password;
};

inline int mdb_server(const SystemConfig& conf)
{
    auto model_id = Catalog::get_model_id(conf.db_directory);

    System system(
        conf.db_directory,
        conf.strings_static_buffer,
        conf.strings_dynamic_buffer,
        conf.versioned_pages_buffer,
        conf.private_pages_buffer,
        conf.unversioned_pages_buffer,
        conf.tensor_store_buffer,
        conf.workers
    );

    MDBServer::Server server;
    if (!conf.admin_user.empty()) {
        server.set_admin_user(conf.admin_user, conf.admin_password);
    }
    try {
        std::unique_ptr<ModelDestroyer> model_destroyer;
        switch (model_id) {
        case Catalog::ModelID::QUAD: {
            std::cout << "Initializing Quad Model..." << std::endl;
            model_destroyer = QuadModel::init();

            quad_model.path_mode = conf.path_mode;
            if (conf.limit != 0) {
                quad_model.MAX_LIMIT = conf.limit;
            }

            quad_model.catalog.print(std::cout);
            server.model_id = MDBServer::Protocol::QUAD_MODEL_ID;
            break;
        }
        case Catalog::ModelID::RDF: {
            std::cout << "Initializing RDF Model..." << std::endl;
            model_destroyer = RdfModel::init();

            rdf_model.path_mode = conf.path_mode;
            if (conf.limit != 0) {
                rdf_model.MAX_LIMIT = conf.limit;
            }

            rdf_model.catalog.print(std::cout);
            server.model_id = MDBServer::Protocol::RDF_MODEL_ID;
            break;
        }
        case Catalog::ModelID::GQL: {
            std::cout << "Initializing GQL Model..." << std::endl;
            model_destroyer = GQLModel::init();

            gql_model.catalog.print(std::cout);
            server.model_id = MDBServer::Protocol::GQL_MODEL_ID;
            break;
        }
        } // end switch
        server.run(conf.port, conf.browser_port, !conf.no_browser, conf.workers, conf.query_timeout);
    } catch (const WrongModelException& e) {
        FATAL_ERROR(e.what());
    } catch (const WrongCatalogVersionException& e) {
        FATAL_ERROR(e.what());
    }

    return EXIT_SUCCESS;
}

inline std::map<std::string, std::function<std::string(SystemConfig&, const std::string&)>>
    get_optionals(bool server)
{
    std::map<std::string, std::function<std::string(SystemConfig&, const std::string&)>> opt;
    if (server) {
        opt.insert({ "--admin-user", [](SystemConfig& config, const std::string& value) {
                        config.admin_user = value;
                        return "";
                    } });
        opt.insert({ "--admin-password", [](SystemConfig& config, const std::string& value) {
                        config.admin_password = value;
                        return "";
                    } });
        opt.insert({ "--port", [](SystemConfig& config, const std::string& value) {
                        try {
                            auto port = std::stoi(value);
                            if (port >= 1024 && port <= 65535) {
                                config.port = port;
                                return "";
                            }
                        } catch (...) {
                        }
                        return "invalid port, expected to be a integer in range 1024 to 65535";
                    } });
        opt.insert({ "--browser-port", [](SystemConfig& config, const std::string& value) {
                        try {
                            auto port = std::stoi(value);
                            if (port >= 1024 && port <= 65535) {
                                config.browser_port = port;
                                return "";
                            }
                        } catch (...) {
                        }
                        return "invalid browser port, expected to be a integer in range 1024 to 65535";
                    } });
        opt.insert({ "--threads", [](SystemConfig& config, const std::string& value) {
                        try {
                            auto threads = std::stoi(value);
                            if (threads > 0) {
                                config.workers = threads;
                                return "";
                            }
                        } catch (...) {
                        }
                        return "invalid worker threads, expected to be a positive integer";
                    } });
    }

    opt.insert({ "--timeout", [](SystemConfig& config, const std::string& value) {
                    try {
                        auto seconds = std::stoi(value);
                        if (seconds > 0) {
                            config.query_timeout = std::chrono::seconds(seconds);
                            return "";
                        }
                    } catch (...) {
                    }
                    return "invalid timeout, expected to be a positive integer";
                } });

    opt.insert({ "--strings-static", [](SystemConfig& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for --strings-static";
                    }
                    config.strings_static_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "--strings-dynamic", [](SystemConfig& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for --strings-dynamic";
                    }
                    config.strings_dynamic_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "--versioned-buffer", [](SystemConfig& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for --versioned-buffer";
                    }
                    config.versioned_pages_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "--unversioned-buffer", [](SystemConfig& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for --unversioned-buffer";
                    }
                    config.unversioned_pages_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "--private-buffer", [](SystemConfig& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for --private-buffer";
                    }
                    config.private_pages_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "--tensor-buffer", [](SystemConfig& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for --tensor-buffer";
                    }
                    config.tensor_store_buffer = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "--path-mode", [](SystemConfig& config, const std::string& value) {
                    auto lc_value = to_lower(value);
                    if (lc_value == "bfs") {
                        config.path_mode = PathSearchMode::BFS;
                    } else if (lc_value == "dfs") {
                        config.path_mode = PathSearchMode::DFS;
                    } else {
                        return "invalid value for --path-mode, expected BFS|DFS";
                    }
                    return "";
                } });

    return opt;
}

inline SystemConfig parse_system_config(const std::vector<std::string>& args, bool server)
{
    SystemConfig config;

    std::vector<std::pair<std::string, std::function<std::string(SystemConfig&, const std::string&)>>>
        positionals;
    auto opt = get_optionals(server);
    std::map<std::string, std::function<void(SystemConfig&)>> flags;
    std::map<std::string, std::string> aliases;

    aliases.insert({ "-t", "--timeout" });

    positionals.push_back({ "db_dir", [](SystemConfig& config, const std::string& value) {
                               config.db_directory = value;
                               return "";
                           } });

    if (server) {
        aliases.insert({ "-p", "--port" });
        aliases.insert({ "-j", "--threads" });
        aliases.insert({ "--workers", "--threads" });

        flags.insert({ "--no-browser", [](SystemConfig& config) {
                          config.no_browser = true;
                      } });
    }

    size_t i = 0;
    size_t current_positional = 0;

    while (i < args.size()) {
        const std::string* arg = &args[i];
        auto found_alias = aliases.find(*arg);
        if (found_alias != aliases.end()) {
            arg = &found_alias->second;
        }

        // search in flags
        if (auto flag_found = flags.find(*arg); flag_found != flags.end()) {
            flag_found->second(config);
        } else if (auto opt_found = opt.find(*arg); opt_found != opt.end()) {
            i++;
            if (i >= args.size()) {
                FATAL_ERROR("expected value for ", args[i]);
            }
            auto error = opt_found->second(config, args[i]);
            if (!error.empty()) {
                FATAL_ERROR(error);
            }
        } else {
            if (arg->size() > 0 && arg->data()[0] == '-') {
                FATAL_ERROR("unrecognized option: ", args[i]);
            }

            if (current_positional >= positionals.size()) {
                FATAL_ERROR("unexpected positional argument: ", args[i]);
            }
            positionals[current_positional].second(config, *arg);
            current_positional++;
        }
        i++;
    }

    if (current_positional < positionals.size()) {
        FATAL_ERROR(
            "missing positional argument: ",
            positionals[current_positional].first,
            "\nRun mdb --help for info."
        );
    }

    return config;
}

inline SystemConfig parse_profile_config(const std::vector<std::string>& args)
{
    SystemConfig config;

    auto opt = get_optionals(false);
    std::map<std::string, std::string> aliases;

    aliases.insert({ "-t", "--timeout" });

    size_t i = 0;

    while (i < args.size()) {
        const std::string* arg = &args[i];
        auto found_alias = aliases.find(*arg);
        if (found_alias != aliases.end()) {
            arg = &found_alias->second;
        }

        if (auto opt_found = opt.find(*arg); opt_found != opt.end()) {
            i++;
            if (i >= args.size()) {
                FATAL_ERROR("expected value for ", args[i]);
            }
            auto error = opt_found->second(config, args[i]);
            if (!error.empty()) {
                FATAL_ERROR(error);
            }
        } else {
            FATAL_ERROR("unrecognized option: ", args[i]);
        }
        i++;
    }

    return config;
}
} // namespace MdbBin
