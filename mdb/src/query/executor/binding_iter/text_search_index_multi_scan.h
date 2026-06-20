#pragma once

#include <memory>
#include <queue>

#include "query/executor/binding_iter.h"
#include "storage/index/text_search/text_search_index.h"

class TextSearchIndexMultiScan : public BindingIter {
public:
    TextSearchIndexMultiScan(
        std::vector<TextSearch::TextSearchIndex*>&& text_search_indexes,
        std::vector<ObjectId>&& predicate_ids,
        const std::string& query,
        TextSearch::SearchType search_type,
        VarId object_var,
        VarId match_var,
        VarId predicate_var
    );

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

private:
    const VarId object_var;
    const VarId match_var;
    const VarId predicate_var;
    const TextSearch::SearchType search_type;
    const std::string query;
    const std::vector<ObjectId> predicate_ids;

    Binding* parent_binding;

    const std::vector<TextSearch::TextSearchIndex*> text_search_indexes;
    std::vector<std::unique_ptr<TextSearch::TextSearchIter>> text_search_iters;
    std::priority_queue<std::pair<double, std::size_t>> score_index_pq;

    // Initializes each text search iter and add them to the score_index_pq if any match is found
    void set_text_search_iters_and_score_index_pq();
};
