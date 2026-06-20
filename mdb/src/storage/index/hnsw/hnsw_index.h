#pragma once

#include <memory>
#include <random>
#include <string>

#include "graph_models/object_id.h"
#include "storage/index/hnsw/hnsw_metric.h"
#include "storage/index/hnsw/hnsw_node.h"
#include "storage/index/hnsw/distance_node_id_map.h"

namespace HNSW {
class HNSWIndex {
    friend class HNSWIndexManager;

public:
    struct HNSWIndexParams {
        uint64_t entry_point_id;  // entry id
        uint64_t dimensions;      // tensor dimensions
        uint64_t layers;          // current number of layers
        uint64_t M;               // max neighbors at construction
        uint64_t ef_construction; // max candidates at construction
    };

    const HNSWIndexParams& get_params() const
    {
        return params;
    };

    static constexpr char DATA_FILENAME[] = "hnsw.dat";

    // create a new index in memory
    // create a new index in memory
    static std::unique_ptr<HNSWIndex> create(
        const std::string& hnsw_index_name,
        uint64_t dimensions,
        uint64_t max_neighbors,
        uint64_t ef_construction,
        MetricFuncType metric_func
    );

    // load an index from disk
    static std::unique_ptr<HNSWIndex> load(const std::string& hnsw_index_name, MetricFuncType metric_func);

    // index all { subject, object } pairs given a predicate
    // find the the top num_neighbors from a pool of num_candidates
    DistanceNodeIdMap query(const float* query_tensor, uint64_t num_neighbors, uint64_t num_candidates);

    // index all { subject, object } pairs given a predicate
    std::tuple<uint_fast32_t> index_predicate(const std::string& predicate);

    // index a single entry
    void index_single(ObjectId ref_object_id, ObjectId tensor_object_id, const float* tensor);

    inline const HNSWNode& get_node(uint64_t node_id) const
    {
        assert(node_id < node_storage.size());
        return node_storage[node_id];
    }

private:
    MetricFuncType metric_func;

    HNSWIndexParams params;

    // useful constants
    uint64_t M0;                // max neighbors per node at layer 0
    uint64_t Mi;                // max neighbors per node at layer i > 0
    double random_layer_factor; // normalization factor for random layer selection

    // used for generating levels
    std::random_device device;
    std::default_random_engine level_generator { device() };
    std::uniform_real_distribution<double> distribution { 0.0l, 1.0l };

    std::vector<HNSWNode> node_storage;

    // node_neighbors_at_layer[i][j] is the map of neighbors for the node i at layer j
    // NOTE: This could be replaced with an array of node_ids, but the set_neighbors_at_layer we
    // would need to calculate the current-worst distance for each node in new_neighbors parameter.
    // This trade-off could be evaluated in the future.
    std::vector<std::vector<DistanceNodeIdMap>> node_neighbors_at_layer;

    // Create constructor
    explicit HNSWIndex(HNSWIndexParams params, MetricFuncType metric_func);

    // Load constructor
    explicit HNSWIndex(
        HNSWIndexParams params,
        MetricFuncType metric_func,
        std::vector<HNSWNode>&& node_storage,
        std::vector<std::vector<DistanceNodeIdMap>>&& node_neighbors_at_layer
    );

    // initialize constants for the index
    inline void init_constants()
    {
        M0 = 2 * params.M;
        Mi = params.M;
        random_layer_factor = 1.0l / std::log(static_cast<double>(params.M));
    }

    // keep only the best num_neighbors from candidates
    inline DistanceNodeIdMap
        slice_neighbors(const DistanceNodeIdMap& candidates, uint64_t num_neighbors) const
    {
        assert(candidates.size() > num_neighbors);

        DistanceNodeIdMap sliced;
        sliced.reserve(num_neighbors);

        auto it = candidates.begin();
        while (num_neighbors > 0) {
            sliced.emplace(*it);
            ++it;
            --num_neighbors;
        }

        return sliced;
    }

    inline uint64_t create_new_node(ObjectId object_oid, ObjectId tensor_oid, uint64_t top_layer)
    {
        const uint64_t node_id = node_storage.size();
        // create node
        node_storage.emplace_back(object_oid, tensor_oid);

        // create node's neighbors
        auto neighbors = std::vector<DistanceNodeIdMap>(top_layer + 1);
        neighbors[0].reserve(M0);
        for (std::size_t i = 1; i < neighbors.size(); ++i) {
            neighbors[i].reserve(Mi);
        }
        node_neighbors_at_layer.emplace_back(std::move(neighbors));

        return node_id;
    }

    // search the top num_neighbors at a layer given a query and a group of entry points
    DistanceNodeIdMap search_at_layer(
        const float* query_float_ptr,
        const std::vector<uint64_t>& entry_point_ids,
        uint64_t num_neighbors,
        uint64_t layer
    );

    // connect the node to the new neighbors (both ways) keeping the max edges limit
    void set_neighbors_at_layer(uint64_t node_id, uint64_t layer, DistanceNodeIdMap&& new_neighbors);

    // generate a random layer number
    inline uint64_t get_random_layer()
    {
        const double random_layer = -std::log(distribution(level_generator)) * random_layer_factor;
        return static_cast<uint64_t>(random_layer);
    }
};
} // namespace HNSW
