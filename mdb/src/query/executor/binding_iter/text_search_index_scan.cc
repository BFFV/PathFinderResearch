#include "text_search_index_scan.h"

void TextSearchIndexScan::_begin(Binding& parent_binding)
{
    // As it is a single scan, the predicate will always be the same
    parent_binding.add(predicate_var, predicate_id);

    this->parent_binding = &parent_binding;

    set_text_search_iter();
}

bool TextSearchIndexScan::_next()
{
    if (text_search_iter->next()) {
        const auto table_pointer = text_search_iter->get_table_pointer();
        const auto table_row = text_search_index_ptr->table->get(table_pointer);
        const ObjectId object_id(table_row[0]);
        const ObjectId match_id(table_row[1]);
        parent_binding->add(object_var, object_id);
        parent_binding->add(match_var, match_id);

        return true;
    }

    return false;
}

void TextSearchIndexScan::_reset()
{
    set_text_search_iter();
}

void TextSearchIndexScan::assign_nulls()
{
    parent_binding->add(object_var, ObjectId::get_null());
    parent_binding->add(match_var, ObjectId::get_null());
    parent_binding->add(predicate_var, ObjectId::get_null());
}

void TextSearchIndexScan::set_text_search_iter()
{
    // TODO: Parametrize allow_errors too
    switch (search_type) {
    case TextSearch::SearchType::Match:
        text_search_iter = text_search_index_ptr->search<TextSearch::SearchType::Match, false>(query);
        break;
    case TextSearch::SearchType::Prefix:
        text_search_iter = text_search_index_ptr->search<TextSearch::SearchType::Prefix, false>(query);
        break;
    default:
        throw QueryExecutionException("Unknown search type");
    }
}

void TextSearchIndexScan::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "TextSearchIndexScan()\n";
}
