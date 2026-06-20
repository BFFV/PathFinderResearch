#pragma once

#include <memory>

#include "query/executor/binding_iter.h"
#include "storage/index/text_search/text_search_index.h"

class TextSearchIndexScan : public BindingIter {
public:
    TextSearchIndexScan(
        TextSearch::TextSearchIndex* text_search_index_,
        const std::string& query_,
        TextSearch::SearchType search_type_,
        VarId object_var_,
        VarId match_var_,
        VarId predicate_var_,
        ObjectId predicate_id_
    ) :
        object_var { object_var_ },
        match_var { match_var_ },
        predicate_var { predicate_var_ },
        predicate_id { predicate_id_ },
        search_type { search_type_ },
        query { query_ },
        text_search_index_ptr { text_search_index_ }
    { }

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

private:
    const VarId object_var;
    const VarId match_var;
    const VarId predicate_var;
    const ObjectId predicate_id;
    const TextSearch::SearchType search_type;
    const std::string query;

    Binding* parent_binding;

    TextSearch::TextSearchIndex* text_search_index_ptr;
    std::unique_ptr<TextSearch::TextSearchIter> text_search_iter;

    // Initializes the text search iter
    void set_text_search_iter();
};
