#pragma once

#include <cassert>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "graph_models/rdf_model/rdf_object_id.h"
#include "query/parser/op/sparql/op.h"

namespace SPARQL {

class OpHNSWSearchIndex : public Op {
public:
    VarId object_var { 0 };
    VarId distance_var { 0 };
    VarId tensor_var { 0 };

    Id query_id { VarId { 0 } };

    uint64_t num_neighbors;
    uint64_t num_candidates;

    std::string hnsw_index_name;

    OpHNSWSearchIndex(VarId object_var_, const std::vector<Id>& args) :
        object_var { object_var_ }
    {
        const auto num_args = args.size();
        if (num_args < 4 || num_args > 6) {
            throw QuerySemanticException("HNSW Search must have 4-6 arguments: <str: hnsw index name> "
                                         "<var|tensor: query> <int: num_neighbors> <int: num_candidates> "
                                         "[<var: distance projection>] [<var: tensor projection>]");
        }

        hnsw_index_name = get_as_string(args[0], "hnsw index name");

        const auto* hnsw_index_ptr = rdf_model.catalog.hnsw_index_manager.get_hnsw_index(hnsw_index_name);
        if (hnsw_index_ptr == nullptr) {
            throw QuerySemanticException("HNSW index not found: \"" + hnsw_index_name + "\"");
        }

        // Get the query
        if (args[1].is_OID()) {
            // Tensor literal query
            query_id = args[1].get_OID();
        } else {
            // Variable query
            query_id = args[1].get_var();
        }

        num_neighbors = get_as_uint64(args[2], "num_neighbors");

        num_candidates = get_as_uint64(args[3], "num_candidates");
        if (num_candidates < num_neighbors) {
            throw QueryException(
                "num_candidates (" + std::to_string(num_candidates)
                + ") must be greater or equal than num_neighbors (" + std::to_string(num_neighbors) + ")"
            );
        }

        // Set optional distance projection
        if (num_args > 4) {
            if (!args[4].is_var()) {
                throw QuerySemanticException(
                    "TextSearchIndex \"distance projection\" argument must be a variable"
                );
            }
            distance_var = args[4].get_var();
        } else {
            distance_var = get_query_ctx().get_internal_var();
        }

        // Set optional tensor projection
        if (num_args > 5) {
            if (!args[5].is_var()) {
                throw QuerySemanticException(
                    "TextSearchIndex \"tensor projection\" argument must be a variable"
                );
            }
            tensor_var = args[5].get_var();
        } else {
            tensor_var = get_query_ctx().get_internal_var();
        }
    }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpHNSWSearchIndex>(*this);
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res = { object_var, distance_var, tensor_var };
        if (query_id.is_var()) {
            res.emplace(query_id.get_var());
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override
    {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override
    {
        return get_all_vars();
    }

    std::set<VarId> get_fixable_vars() const override
    {
        if (query_id.is_var()) {
            return { query_id.get_var() };
        }
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ') << "OpHNSWSearchIndex(";
        os << object_var << ", \"";
        os << hnsw_index_name << "\", ";
        if (query_id.is_OID()) {
            os << query_id.get_OID();
        } else {
            os << query_id.get_var();
        }
        os << ", " << object_var << ", ";
        os << distance_var << ", ";
        os << tensor_var;
        return os << ")";
    }

private:
    // Just for logging
    static inline std::string get_as_string(Id id, const std::string& argument_name)
    {
        const auto throw_error = [&argument_name]() {
            throw QuerySemanticException(
                "HNSWSearchIndex \"" + argument_name + "\" argument must be a string"
            );
        };

        if (!id.is_OID()) {
            throw_error();
        }

        const auto oid = id.get_OID();

        if (RDF_OID::get_generic_type(oid) != RDF_OID::GenericType::STRING) {
            throw_error();
        }

        return SPARQL::Conversions::unpack_string(oid);
    }

    static inline uint64_t get_as_uint64(Id id, const std::string& argument_name)
    {
        const auto throw_error = [&argument_name]() {
            throw QuerySemanticException(
                "HNSWSearchIndex \"" + argument_name + "\" argument must be a positive integer"
            );
        };

        if (!id.is_OID()) {
            throw_error();
        }

        const auto oid = id.get_OID();

        if (RDF_OID::get_generic_sub_type(oid) != RDF_OID::GenericSubType::INTEGER) {
            throw_error();
        }

        const auto res = SPARQL::Conversions::unpack_int(oid);
        if (res < 0) {
            throw_error();
        }

        return static_cast<uint64_t>(res);
    }
};
} // namespace SPARQL
