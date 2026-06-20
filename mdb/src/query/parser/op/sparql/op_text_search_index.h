#pragma once

#include <cassert>
#include <ctype.h>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "graph_models/rdf_model/rdf_object_id.h"
#include "query/parser/op/sparql/op.h"
#include "query/query_context.h"
#include "storage/index/text_search/search_type.h"
#include "storage/index/text_search/text_search_index.h"

namespace SPARQL {

class OpTextSearchIndex : public Op {
public:
    VarId object_var;
    VarId match_var;
    VarId predicate_var;

    std::string text_search_index_name;
    std::string query;

    TextSearch::SearchType search_type;

    OpTextSearchIndex(VarId object_var_, const std::vector<Id>& args) :
        object_var { object_var_ },
        match_var { 0 },    // Must be set later
        predicate_var { 0 } // Must be set later
    {
        const auto num_args = args.size();
        if (num_args < 2 || num_args > 5) {
            throw QuerySemanticException(
                "TextSearchIndex must have 2-5 arguments: <str: text search index name | \"*\"> "
                "<str: query> [<str: search type>] [<var: match projection>] [<var: predicate projection>]"
            );
        }

        const auto text_search_index_name_oid = check_string_type(args[0], "text search index name");
        text_search_index_name = SPARQL::Conversions::unpack_string(text_search_index_name_oid);
        if (text_search_index_name == "*") {
            // Will use the all text search indexes, at least one must exist
            if (rdf_model.catalog.text_search_index_manager.num_text_search_indexes() == 0) {
                throw QuerySemanticException("Cannot use \"*\" when no TextSearchIndex exists");
            }
        } else {
            // Will use an specific text search index
            const auto* text_search_index_ptr = rdf_model.catalog.text_search_index_manager
                                                    .get_text_search_index(text_search_index_name);
            if (text_search_index_ptr == nullptr) {
                throw QuerySemanticException("TextSearchIndex not found: \"" + text_search_index_name + "\"");
            }
        }

        // Get the query text
        const auto query_oid = check_string_type(args[1], "query");
        query = SPARQL::Conversions::unpack_string(query_oid);

        // Set optional search type
        if (num_args > 2) {
            // Parse search type
            const auto search_type_oid = check_string_type(args[2], "search type");
            auto search_type_str = SPARQL::Conversions::unpack_string(search_type_oid);
            std::transform(search_type_str.begin(), search_type_str.end(), search_type_str.begin(), tolower);
            if (search_type_str == "match") {
                search_type = TextSearch::SearchType::Match;
            } else if (search_type_str == "prefix") {
                search_type = TextSearch::SearchType::Prefix;
            } else {
                throw QuerySemanticException("Unknown search type: " + search_type_str);
            }
        } else {
            // Default search type
            search_type = TextSearch::SearchType::Prefix;
        }

        // Set optional match projection
        if (num_args > 3) {
            // Parse match projection
            if (!args[3].is_var()) {
                throw QuerySemanticException(
                    "TextSearchIndex \"match projection\" argument must be a variable"
                );
            }
            match_var = args[3].get_var();
        } else {
            // Will not project the matched string
            match_var = get_query_ctx().get_internal_var();
        }

        if (num_args > 4) {
            // Parse predicate projection
            if (!args[4].is_var()) {
                throw QuerySemanticException(
                    "TextSearchIndex \"predicate projection\" argument must be a variable"
                );
            }
            predicate_var = args[4].get_var();
        } else {
            // Will not project the predicate
            predicate_var = get_query_ctx().get_internal_var();
        }
    }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpTextSearchIndex>(*this);
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return { object_var, match_var, predicate_var };
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
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ') << "OpTextSearchIndex(";
        os << object_var << ", \"";
        os << text_search_index_name << "\", \"";
        os << query << "\", ";
        os << search_type << ", ";
        os << match_var << ", ";
        os << predicate_var;
        return os << "))";
    }

private:
    // Just for logging
    static inline ObjectId check_string_type(Id id, const std::string& argument_name)
    {
        if (id.is_var()) {
            throw QuerySemanticException(
                "TextSearchIndex \"" + argument_name + "\" argument must be a string"
            );
        }

        const auto oid = id.get_OID();

        if (RDF_OID::get_generic_type(oid) != RDF_OID::GenericType::STRING) {
            throw QuerySemanticException(
                "TextSearchIndex \"" + argument_name + "\" argument must be a string"
            );
        }

        return oid;
    }
};
} // namespace SPARQL
