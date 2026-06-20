#pragma once

#include <boost/container/flat_map.hpp>

namespace HNSW {

using DistanceNodeIdMap = boost::container::flat_multimap<float, uint64_t>;

} // namespace HNSW
