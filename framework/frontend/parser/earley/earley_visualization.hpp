//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __EARLEY_VISUALIZATION_H__
#define __EARLEY_VISUALIZATION_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
class earley_visualization : private noncopyable
{
    public:
        using symbol_type = rule::symbol_type;
        using symbols_type = rule::symbols_type;

        using set_type = symbol::set_type;
        using sets_type = symbol::sets_type;

        using rule_type = grammar::rule_type;
        using rules_type = grammar::rules_type;

        using earley_parser_type = T;

        using item_type = typename earley_parser_type::item_type;
        using chart_type = typename earley_parser_type::chart_type;
        using charts_type = typename earley_parser_type::charts_type;

        using tree_type = typename earley_parser_type::tree_type;
        using trees_type = typename earley_parser_type::trees_type;

    private:
        static void collect_dot_labels(const tree_type& tree, std::size_t k, std::wostream& stream);

    public:
        static string_type decorate_item(const item_type& item, bool recursive = true);
        static string_type decorate_chart(const chart_type& chart);
        static string_type decorate_charts(const charts_type& charts);

        static void print_tree(const tree_type& tree, std::size_t level, std::wostream& stream);
        static void print_tree(const trees_type& trees, std::wostream& stream);

        static void decorate_tree(const tree_type& tree, std::wostream& stream);
        static void decorate_trees(const trees_type& trees, const string_type& dot_file_name);
};

END_NAMESPACE

#endif // __EARLEY_VISUALIZATION_H__
