#pragma once

#include "graph_models/common/conversions.h"
#include "graph_models/common/datatypes/tensor.h"
#include "graph_models/rdf_model/rdf_object_id.h"
#include "query/executor/binding_iter.h"
#include "query/var_id.h"
#include "storage/index/hnsw/hnsw_index.h"

class HNSWIndexScanFixedQuery : public BindingIter {
public:
    HNSWIndexScanFixedQuery(
        VarId object_var_,
        VarId distance_var_,
        VarId tensor_var_,
        ObjectId query_oid_,
        uint64_t num_neighbors_,
        uint64_t num_candidates_,
        HNSW::HNSWIndex* hnsw_index_ptr_
    ) :
        object_var { object_var_ },
        distance_var { distance_var_ },
        tensor_var { tensor_var_ },
        query_oid { query_oid_ },
        num_neighbors { num_neighbors_ },
        num_candidates { num_candidates_ },
        hnsw_index_ptr { hnsw_index_ptr_ }
    {
        switch (RDF_OID::get_generic_sub_type(query_oid)) {
        case RDF_OID::GenericSubType::TENSOR_FLOAT:
        case RDF_OID::GenericSubType::TENSOR_DOUBLE: {
            query = Common::Conversions::to_tensor<float>(query_oid);
            break;
        }
        default:
            // Empty tensor to mark that the input was invalid
            break;
        }

        if (query.size() != hnsw_index_ptr->get_params().dimensions) {
            dimension_mismatch = true;
        }
    }

    void print(std::ostream& os, int indent, bool stats) const override;

    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    const VarId object_var;
    const VarId distance_var;
    const VarId tensor_var;
    const ObjectId query_oid;
    const uint64_t num_neighbors;
    const uint64_t num_candidates;

private:
    // If there is a dimension mismatch, this operator will not yield results
    bool dimension_mismatch { false };

    // Keep the query unpacked instead of unpacking on each _next() call
    Tensor<float> query;

    HNSW::HNSWIndex* const hnsw_index_ptr;

    Binding* parent_binding;

    HNSW::DistanceNodeIdMap map;

    HNSW::DistanceNodeIdMap::iterator map_it;
};
