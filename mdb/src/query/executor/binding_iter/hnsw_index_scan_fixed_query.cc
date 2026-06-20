#include "hnsw_index_scan_fixed_query.h"

#include "graph_models/common/conversions.h"

void HNSWIndexScanFixedQuery::_begin(Binding& parent_binding)
{
    this->parent_binding = &parent_binding;

    if (dimension_mismatch) {
        return;
    }

    map = hnsw_index_ptr->query(query.data(), num_neighbors, num_candidates);
    map_it = map.begin();
    map = hnsw_index_ptr->query(query.data(), num_neighbors, num_candidates);
    map_it = map.begin();
}

bool HNSWIndexScanFixedQuery::_next()
{
    if (dimension_mismatch) {
        return false;
    }

    if (map_it != map.end()) {
        const auto [distance, node_id] = *map_it;
        const HNSW::HNSWNode& node = hnsw_index_ptr->get_node(node_id);
        parent_binding->add(distance_var, Common::Conversions::pack_float(distance));
        parent_binding->add(object_var, node.object_oid);
        parent_binding->add(tensor_var, node.tensor_oid);
        ++map_it;
        parent_binding->add(object_var, node.object_oid);
        parent_binding->add(tensor_var, node.tensor_oid);
        ++map_it;
        return true;
    }

    return false;
}

void HNSWIndexScanFixedQuery::_reset()
{
    if (dimension_mismatch) {
        return;
    }

    map_it = map.begin();
}

void HNSWIndexScanFixedQuery::assign_nulls()
{
    parent_binding->add(object_var, ObjectId::get_null());
    parent_binding->add(distance_var, ObjectId::get_null());
    parent_binding->add(tensor_var, ObjectId::get_null());
}

void HNSWIndexScanFixedQuery::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "HNSWIndexScanFixedQuery(";
    os << "object_var: " << object_var;
    os << " distance_var: " << distance_var;
    os << " tensor_var: " << tensor_var;
    os << " num_neighbors: " << num_neighbors;
    os << " num_candidates: " << num_candidates;
    os << " query: " << query_oid;
    os << ")\n";
}
