﻿//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\unicode.hpp>
#include <core\text.hpp>
#include <core\domain_helper.hpp>
#include <core\logger.hpp>
#include <core\data_provider.hpp>
#include <core\file_data_provider.hpp>
#include <core\content.hpp>
#include <core\counter.hpp>
#include <core\factory.hpp>
#include <core\singleton.hpp>
#include <core\enum.hpp>
#include <core\flags.hpp>
#include <core\command_line.hpp>
#include <core\configurator.hpp>

#include <core\tree.hpp>
#include <core\visitor.hpp>

#include <frontend\grammar\token.hpp>
#include <frontend\grammar\symbol.hpp>
#include <frontend\grammar\rule.hpp>
#include <frontend\grammar\grammar.hpp>
#include <frontend\grammar\grammar_algorithm.hpp>
#include <frontend\grammar\grammar_visualization.hpp>

#include <frontend\fsa\fsa_transition.hpp>
#include <frontend\fsa\fsa_state.hpp>
#include <frontend\fsa\fsa.hpp>
#include <frontend\fsa\fsa_re.hpp>
#include <frontend\fsa\fsa_state_set.hpp>
#include <frontend\fsa\fsa_algorithm.hpp>

#include <frontend\fsa\fsa_visualization.hpp>
#include <frontend\fsa\fsa_codegen.hpp>

#include <core\data_provider.hpp>
#include <core\file_data_provider.hpp>
#include <core\content.hpp>

#include <core\diagnostics.hpp>

#include <frontend\lexical_analyzer\lexical_analyzer.hpp>

#include <frontend\tree\ast.hpp>
#include <frontend\tree\cst.hpp>

#include <frontend\parser\parser_algorithm.hpp>

#include <frontend\parser\parser.hpp>
#include <frontend\parser\parser.inl>

#include <frontend\parser\lr\lr_algorithm.hpp>
#include <frontend\parser\lr\lr_visualization.hpp>
#include <frontend\parser\lr\lr_parser.hpp>
#include <frontend\parser\lr\lr_parser.inl>

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

void test_lr_table()
{
    grammar gr;

    gr.load(LR"(D:\Projects\fe\grammars\LR.Aho.G5.29.txt)");

    std::wcout << grammar_visualization::decorate_grammar(gr) << std::endl;

    uint8_t k = 1;

    {
        lr_algorithm::lr_items_type items;

        grammar_algorithm::symbols_type symbols;
        symbols.emplace_back(symbol::epsilon);

        lr_algorithm::build_lr_items_set(gr, symbols, k, items);
    }
    {
        lr_algorithm::lr_items_type items;

        grammar_algorithm::symbols_type symbols;
        symbols.emplace_back(gr.pool()[L"S"]);

        lr_algorithm::build_lr_items_set(gr, symbols, k, items);
    }
    {
        lr_algorithm::lr_items_type items;

        grammar_algorithm::symbols_type symbols;
        symbols.emplace_back(gr.pool()[L"S"]);
        symbols.emplace_back(gr.pool()[L"a"]);

        lr_algorithm::build_lr_items_set(gr, symbols, k, items);
    }

    lr_algorithm::lr_action_table_type action_table;
    lr_algorithm::lr_goto_table_type goto_table;

    lr_algorithm::build_lr_table(gr, k, action_table, goto_table);
}
