#include "text_search_index_multi_scan.h"

TextSearchIndexMultiScan::TextSearchIndexMultiScan(
    std::vector<TextSearch::TextSearchIndex*>&& text_search_indexes_,
    std::vector<ObjectId>&& predicate_ids_,
    const std::string& query_,
    TextSearch::SearchType search_type_,
    VarId object_var_,
    VarId match_var_,
    VarId predicate_var_
) :
    object_var { object_var_ },
    match_var { match_var_ },
    predicate_var { predicate_var_ },
    search_type { search_type_ },
    query { query_ },
    predicate_ids { std::move(predicate_ids_) },
    text_search_indexes { std::move(text_search_indexes_) }
{ }

void TextSearchIndexMultiScan::_begin(Binding& parent_binding)
{
    this->parent_binding = &parent_binding;

    set_text_search_iters_and_score_index_pq();
}

bool TextSearchIndexMultiScan::_next()
{
    if (score_index_pq.empty()) {
        // All iterators are exhausted
        return false;
    }

    // Get the iterator with the highest score
    auto&& [_, index] = std::move(score_index_pq.top());
    score_index_pq.pop();

    const auto& text_search_iter = text_search_iters[index];
    const auto& text_search_index = text_search_indexes[index];

    const auto table_pointer = text_search_iter->get_table_pointer();
    const auto table_row = text_search_index->table->get(table_pointer);
    const ObjectId object_id(table_row[0]);
    // const ObjectId predicate_id("google.com");
    const ObjectId match_id(table_row[1]);
    parent_binding->add(object_var, object_id);
    parent_binding->add(match_var, match_id);
    parent_binding->add(predicate_var, predicate_ids[index]);

    if (text_search_iter->next()) {
        // Update the priority queue with the advanced iterator
        const auto next_score = text_search_iter->get_score();
        score_index_pq.emplace(next_score, index);
    }

    return true;
}

void TextSearchIndexMultiScan::_reset()
{
    set_text_search_iters_and_score_index_pq();
}

void TextSearchIndexMultiScan::assign_nulls()
{
    parent_binding->add(object_var, ObjectId::get_null());
    parent_binding->add(match_var, ObjectId::get_null());
    parent_binding->add(predicate_var, ObjectId::get_null());
}

void TextSearchIndexMultiScan::set_text_search_iters_and_score_index_pq()
{
    // Clear containers
    text_search_iters.clear();
    score_index_pq = {};
    for (std::size_t i = 0; i < text_search_indexes.size(); ++i) {
        std::unique_ptr<TextSearch::TextSearchIter> text_search_iter;
        // TODO: Parametrize allow_errors too
        switch (search_type) {
        case TextSearch::SearchType::Match:
            text_search_iter = text_search_indexes[i]->search<TextSearch::SearchType::Match, false>(query);
            break;
        case TextSearch::SearchType::Prefix:
            text_search_iter = text_search_indexes[i]->search<TextSearch::SearchType::Prefix, false>(query);
            break;
        default:
            throw QueryExecutionException("Unknown search type");
        }

        if (text_search_iter->next()) {
            // Init with first score
            const auto score = text_search_iter->get_score();
            text_search_iters.emplace_back(std::move(text_search_iter));
            score_index_pq.emplace(score, i);
        } else {
            // No matches, no need to add to priority queue
            text_search_iters.emplace_back(nullptr);
        }
    }
}

void TextSearchIndexMultiScan::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "TextSearchIndexMultiScan()\n";
}
