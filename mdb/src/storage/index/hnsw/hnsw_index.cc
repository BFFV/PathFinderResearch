#include "hnsw_index.h"

#include <filesystem>
#include <memory>

#include <boost/unordered/unordered_flat_set.hpp>

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "query/optimizer/rdf_model/plan/triple_plan.h"
#include "query/query_context.h"
#include "storage/index/hnsw/distance_node_id_map.h"
#include "storage/index/hnsw/hnsw_index_manager.h"
#include "system/file_manager.h"

namespace fs = std::filesystem;

namespace HNSW {
std::unique_ptr<HNSWIndex> HNSWIndex::create(
    const std::string& hnsw_index_name,
    uint64_t dimensions,
    uint64_t max_neighbors,
    uint64_t n_candidates_insertion,
    MetricFuncType metric_func
)
{
    const auto relative_index_path = fs::path(HNSWIndexManager::HNSW_INDEX_DIR) / hnsw_index_name;
    const auto absolute_index_path = file_manager.get_file_path(relative_index_path);

    if (!fs::create_directories(absolute_index_path)) {
        throw std::runtime_error("Could not create directories: " + absolute_index_path);
    };

    HNSWIndexParams params {};
    params.entry_point_id = 0;
    params.dimensions = dimensions;
    params.layers = 1;
    params.M = max_neighbors;
    params.ef_construction = n_candidates_insertion;

    return std::unique_ptr<HNSWIndex>(new HNSWIndex(params, metric_func));
}

std::unique_ptr<HNSWIndex> HNSWIndex::load(const std::string& hnsw_index_name, MetricFuncType metric_func)
{
    const auto relative_index_path = fs::path(HNSWIndexManager::HNSW_INDEX_DIR) / hnsw_index_name;
    const auto absolute_index_path = file_manager.get_file_path(relative_index_path);
    const auto absolute_data_file_path = fs::path(absolute_index_path) / HNSWIndex::DATA_FILENAME;

    std::fstream ifs(absolute_data_file_path, std::ios::in | std::ios::binary);

    // load params
    HNSWIndex::HNSWIndexParams params {};
    ifs.read(reinterpret_cast<char*>(&params), sizeof(HNSWIndex::HNSWIndexParams));
    if (!ifs.good()) {
        throw std::runtime_error("Could not read params file");
    }

    uint64_t num_nodes;
    ifs.read(reinterpret_cast<char*>(&num_nodes), sizeof(uint64_t));
    if (!ifs.good()) {
        throw std::runtime_error("Could not read params file");
    }

    std::vector<HNSWNode> node_storage;
    std::vector<std::vector<DistanceNodeIdMap>> node_neighbors;

    node_storage.reserve(num_nodes);
    node_neighbors.reserve(num_nodes);

    for (uint64_t node_id = 0; node_id < num_nodes; ++node_id) {
        // read node
        HNSWNode node {};
        ifs.read(reinterpret_cast<char*>(&node), sizeof(HNSWNode));
        if (!ifs.good()) {
            throw std::runtime_error("Could not read params file");
        }
        node_storage.emplace_back(std::move(node));

        // read neighbors
        uint64_t num_layers;
        ifs.read(reinterpret_cast<char*>(&num_layers), sizeof(uint64_t));
        if (!ifs.good()) {
            throw std::runtime_error("Could not read params file");
        }

        node_neighbors.emplace_back(std::vector<DistanceNodeIdMap>(num_layers));

        for (uint64_t layer_i = 0; layer_i < num_layers; ++layer_i) {
            uint64_t num_neighbors_at_layer_i;
            ifs.read(reinterpret_cast<char*>(&num_neighbors_at_layer_i), sizeof(uint64_t));
            if (!ifs.good()) {
                throw std::runtime_error("Could not read params file");
            }

            node_neighbors[node_id][layer_i].reserve(num_neighbors_at_layer_i);

            for (uint64_t neighbor_i = 0; neighbor_i < num_neighbors_at_layer_i; ++neighbor_i) {
                float neighbor_dist;
                ifs.read(reinterpret_cast<char*>(&neighbor_dist), sizeof(float));
                if (!ifs.good()) {
                    throw std::runtime_error("Could not read params file");
                }
                uint64_t neighbor_id;
                ifs.read(reinterpret_cast<char*>(&neighbor_id), sizeof(uint64_t));
                if (!ifs.good()) {
                    throw std::runtime_error("Could not read params file");
                }
                node_neighbors[node_id][layer_i].emplace(neighbor_dist, neighbor_id);
            }
        }
    }

    return std::unique_ptr<HNSWIndex>(
        new HNSWIndex(params, metric_func, std::move(node_storage), std::move(node_neighbors))
    );
}

DistanceNodeIdMap
    HNSWIndex::query(const float* query_float_ptr, uint64_t num_neighbors, uint64_t num_candidates)
{
    std::vector<uint64_t> entry_point_ids = { params.entry_point_id };

    const uint64_t top_layer = params.layers - 1;

    // Find best node until layer 0
    for (uint64_t current_layer = top_layer; current_layer > 0; --current_layer) {
        DistanceNodeIdMap
            current_layer_nns = search_at_layer(query_float_ptr, entry_point_ids, 1, current_layer);
        entry_point_ids = { current_layer_nns.begin()->second };
    }

    DistanceNodeIdMap
        nearest_neighbors = search_at_layer(query_float_ptr, entry_point_ids, num_candidates, 0);

    if (nearest_neighbors.size() > num_candidates) {
        nearest_neighbors = slice_neighbors(nearest_neighbors, num_neighbors);
    }

    return nearest_neighbors;
}

std::tuple<uint_fast32_t> HNSWIndex::index_predicate(const std::string& predicate)
{
    const auto subject_var = get_query_ctx().get_internal_var();
    const auto predicate_val = SPARQL::Conversions::pack_iri(predicate);
    const auto object_var = get_query_ctx().get_internal_var();

    const auto triple_plan = SPARQL::TriplePlan(subject_var, predicate_val, object_var);
    auto triple_plan_iter = triple_plan.get_binding_iter();

    Binding binding(get_query_ctx().get_var_size());
    triple_plan_iter->begin(binding);

    const auto num_expected_insertions = rdf_model.catalog.get_predicate_count(predicate_val.id);
    node_storage.reserve(num_expected_insertions);
    node_neighbors_at_layer.reserve(num_expected_insertions);

    uint_fast32_t total_inserted_elements { 0 };
    while (triple_plan_iter->next()) {
        const auto object_oid = binding[object_var];

        const auto gen_t = RDF_OID::get_generic_type(object_oid);
        if (gen_t != RDF_OID::GenericType::TENSOR) {
            // Object is not a tensor
            continue;
        }

        const auto object_tensor = Common::Conversions::to_tensor<float>(object_oid);
        if (object_tensor.size() != params.dimensions) {
            // Tensor dimension does not match
            continue;
        }

        const auto subject_oid = binding[subject_var];
        index_single(subject_oid, object_oid, object_tensor.data());
        ++total_inserted_elements;
    }

    return { total_inserted_elements };
}

void HNSWIndex::index_single(ObjectId ref_object_id, ObjectId tensor_object_id, const float* query_float_ptr)
{
    // Create new node at a random layer
    const uint64_t node_top_layer = get_random_layer();
    const uint64_t node_id = create_new_node(ref_object_id, tensor_object_id, node_top_layer);

    // Initialize entry points
    std::vector<uint64_t> entry_point_ids = { params.entry_point_id };

    // Update the best entry point for layers in range [top_layer, node_top_layer)
    const uint64_t top_layer = params.layers - 1;
    for (uint64_t current_layer = top_layer; current_layer > node_top_layer; --current_layer) {
        const DistanceNodeIdMap
            current_layer_nns = search_at_layer(query_float_ptr, entry_point_ids, 1, current_layer);
        entry_point_ids = { current_layer_nns.begin()->second };
    }

    // Insert in node's layer and the layers below
    for (int64_t current_layer = std::min(node_top_layer, top_layer); current_layer >= 0; --current_layer) {
        DistanceNodeIdMap current_layer_nns = search_at_layer(
            query_float_ptr,
            entry_point_ids,
            params.ef_construction,
            current_layer
        );
        const uint64_t max_neighbors = current_layer == 0 ? M0 : Mi;

        if (current_layer_nns.size() > max_neighbors) {
            current_layer_nns = slice_neighbors(current_layer_nns, max_neighbors);
        }

        // Update entry points
        entry_point_ids.clear();
        entry_point_ids.reserve(current_layer_nns.size());
        for (const auto& [_, current_layer_nn_node_id] : current_layer_nns) {
            entry_point_ids.emplace_back(current_layer_nn_node_id);
        }

        // Mutually connect node with its new neighbors
        set_neighbors_at_layer(node_id, current_layer, std::move(current_layer_nns));
    }

    if (node_top_layer > top_layer) {
        params.entry_point_id = node_id;
        params.layers = node_top_layer + 1;
    };
}

HNSWIndex::HNSWIndex(HNSWIndexParams params_, MetricFuncType metric_func_) :
    metric_func { metric_func_ },
    params { params_ }
{
    init_constants();
}

HNSWIndex::HNSWIndex(
    HNSWIndexParams params_,
    MetricFuncType metric_func_,
    std::vector<HNSWNode>&& node_storage_,
    std::vector<std::vector<DistanceNodeIdMap>>&& node_neighbors_at_layer_
) :
    metric_func { metric_func_ },
    params { params_ },
    node_storage { std::move(node_storage_) },
    node_neighbors_at_layer { std::move(node_neighbors_at_layer_) }
{
    init_constants();
}

DistanceNodeIdMap HNSWIndex::search_at_layer(
    const float* query_float_ptr,
    const std::vector<uint64_t>& entry_point_ids,
    uint64_t num_neighbors,
    uint64_t layer
)
{
    assert(!entry_point_ids.empty());
    assert(num_neighbors > 0);

    boost::unordered_flat_set<uint64_t> visited_set;

    DistanceNodeIdMap candidates;
    DistanceNodeIdMap top_k;

    visited_set.reserve(num_neighbors);
    candidates.reserve(num_neighbors);
    top_k.reserve(num_neighbors);

    // Explore each entry point
    for (const auto& entry_id : entry_point_ids) {
        const auto entry_point_tensor = Common::Conversions::to_tensor<float>(
            node_storage[entry_id].tensor_oid
        );
        const auto entry_point_dist = metric_func(
            query_float_ptr,
            entry_point_tensor.data(),
            params.dimensions
        );

        visited_set.emplace(entry_id);
        candidates.emplace(entry_point_dist, entry_id);
        top_k.emplace(entry_point_dist, entry_id);
    }

    float worst_distance = (--top_k.end())->first;
    while (!candidates.empty()) {
        // Get best candidate
        const auto [candidate_dist, candidate_id] = *candidates.begin();

        if (top_k.size() == num_neighbors && candidate_dist > worst_distance) {
            // we have enough candidates and there are no better ones
            break;
        }
        candidates.erase(candidates.begin());

        for (const auto& [_, candidate_neighbor_id] : node_neighbors_at_layer[candidate_id][layer]) {
            if (visited_set.contains(candidate_neighbor_id)) {
                continue;
            }
            visited_set.emplace(candidate_neighbor_id);

            const auto candidate_neighbor_tensor = Common::Conversions::to_tensor<float>(
                node_storage[candidate_neighbor_id].tensor_oid
            );
            const float candidate_neighbor_dist = metric_func(
                query_float_ptr,
                candidate_neighbor_tensor.data(),
                params.dimensions
            );

            if (top_k.size() < num_neighbors || candidate_neighbor_dist < worst_distance) {
                candidates.emplace(candidate_neighbor_dist, candidate_neighbor_id);
                top_k.emplace(candidate_neighbor_dist, candidate_neighbor_id);

                if (top_k.size() > num_neighbors) {
                    // Remove worst candidate if limit reached
                    top_k.erase(--top_k.end());
                }

                worst_distance = (--top_k.end())->first;
            }
        }
    }

    return top_k;
}

void HNSWIndex::set_neighbors_at_layer(uint64_t node_id, uint64_t layer, DistanceNodeIdMap&& new_neighbors)
{
    const auto max_neighbors = layer == 0 ? M0 : Mi;
    assert(new_neighbors.size() <= max_neighbors);

    // neighbors -> node
    for (const auto& [new_neighbor_dist, new_neighbor_id] : new_neighbors) {
        DistanceNodeIdMap& new_neighbor_neighbors_at_layer = node_neighbors_at_layer[new_neighbor_id][layer];
        if (new_neighbor_neighbors_at_layer.size() < max_neighbors) {
            // max_neighbors not reached, just add the connection
            new_neighbor_neighbors_at_layer.emplace(new_neighbor_dist, node_id);
        } else {
            // replace if the connection ony if it is better than the worst one
            const auto last_it = --new_neighbor_neighbors_at_layer.end();
            if (new_neighbor_dist < last_it->first) {
                new_neighbor_neighbors_at_layer.erase(last_it);
                new_neighbor_neighbors_at_layer.emplace(new_neighbor_dist, node_id);
            }
        }
    }

    // node -> neighbors
    node_neighbors_at_layer[node_id][layer] = std::move(new_neighbors);
}

} // namespace HNSW
