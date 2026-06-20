#pragma once

#include <cstdint>
#include <ostream>
#include <string>

/**
 * Helper struct for logging text search index updates
 */

struct HNSWIndexUpdateData {
    std::string hnsw_index_name;

    bool created { false };

    uint_fast32_t inserted_elements { 0 };

    friend std::ostream& operator<<(std::ostream& os, const HNSWIndexUpdateData& data)
    {
        os << "{\"name\": \"" << data.hnsw_index_name << "\"";
        os << ", \"created\": " << (data.created ? "true" : "false");
        os << ", \"inserted_elements\": " << data.inserted_elements;
        os << "}";
        return os;
    }
};
