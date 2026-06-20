#pragma once

#include "graph_models/object_id.h"

namespace HNSW {
struct HNSWNode {
    ObjectId object_oid; // Object id itself
    ObjectId tensor_oid; // Tensor associated to the object_oid

    HNSWNode() = default;

    HNSWNode(ObjectId object_oid_, ObjectId tensor_oid_) :
        object_oid { object_oid_ },
        tensor_oid { tensor_oid_ }
    { }
};
} // namespace HNSW
