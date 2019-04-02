﻿//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\enumerate.hpp>
#include <core\unicode.hpp>
#include <core\text.hpp>
#include <core\domain_helper.hpp>
#include <core\logger.hpp>
#include <core\data_provider.hpp>
#include <core\file_data_provider.hpp>
#include <core\string_data_provider.hpp>
#include <core\content.hpp>
#include <core\counter.hpp>
#include <core\factory.hpp>
#include <core\singleton.hpp>
#include <core\enum.hpp>
#include <core\flags.hpp>
#include <core\command_line.hpp>
#include <core\configurator.hpp>

#include <core\tree.hpp>
#include <core\dag.hpp>

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
#include <core\statistics.hpp>

#include <frontend\lexical_analyzer\lexical_analyzer.hpp>

#include <frontend\parser\parser_algorithm.hpp>

#include <frontend\parser\parser_tree.hpp>
#include <frontend\parser\parser_dag.hpp>

#include <frontend\parser\parser.hpp>
#include <frontend\parser\parser.inl>

#include <frontend\parser\earley\earley_parser.hpp>
#include <frontend\parser\earley\earley_parser.inl>

#include <frontend\parser\earley\earley_visualization.hpp>
#include <frontend\parser\earley\earley_visualization.inl>

#include <backend\ir\ir.hpp>
#include <backend\ir\ir.inl>

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)

struct earley_token_traits// : public token_traits
{
    DECLARE_ENUM
    (
        type,
        uint32_t,
        unknown = 0,
        epsilon = 5,
        eol,
        eos,

        a,
        b,
        c,
        d,
        e,
        f,
        g,
        n,
        p, // +
        m, // *
        l, // (
        r, // )

        // the following one (1) entry MUST be the last entry in the enum
        size
    )
};

earley_token_traits::enum_map_type earley_token_traits::mapping;

class earley_lexical_analyzer : public lexical_analyzer<token<earley_token_traits>>
{
    private:
        std::size_t k = 0;
        const grammar& my_gr;

    protected:
        virtual void next_lexeme_impl() override
        {
            if(k == (*content()).count())
            {
                my_token.type = token_type::traits::type::eos;
            }
            else
            {
                string_type name(text::codepoint_to_string((*content()).data()[k]));
                auto& symbol((*(my_gr.pool().find(name))).second);
                if(name == L"+")
                    name = L"p";
                else if(name == L"*")
                    name = L"m";
                else if(name == L"(")
                    name = L"l";
                else if(name == L")")
                    name = L"r";
                my_token.type = token_type::traits::value(name);
                (*symbol).id() = static_cast<uint32_t>(my_token.type);
            }

            k++;
        }

    public:
        earley_lexical_analyzer(const grammar& gr, const content_type& content) : my_gr(gr), lexical_analyzer(content)
        {
            earley_token_traits::initialize();
        }
};

class my_earley_parser : public earley_parser<token<earley_token_traits>>
{
    public:
        using eparser_type = earley_parser<token<earley_token_traits>>;

    public:
        my_earley_parser(const lexical_analyzer_type& lexical_analyzer, grammar& gr, earley_parser::tree_kind kind) : earley_parser(lexical_analyzer, gr, kind)
        {
        }

        tree_type handle_start(const item_type& item) override
        {
            auto result(factory::create<parser_tree<token_type>>());
            (*result).symbol = ((*(*item).rule).lhs()[0]);
            return result;
        }

        tree_type handle_terminal(const symbol_type& symbol, const token_type& token, const tree_type& node) override
        {
            auto result(factory::create<parser_tree<token_type>>());
            (*result).symbol = symbol;
            (*result).token = token;
            (*result).papa = node;
            (*node).kids.emplace_back(result);
            return result;
        }

        tree_type handle_before_nonterminal(const item_type& item, const tree_type& node, bool) override
        {
            auto result(factory::create<parser_tree<token_type>>());
            (*result).symbol = ((*(*item).rule).lhs()[0]);
            (*result).papa = node;
            (*node).kids.emplace_back(result);
            return result;
        }

        tree_type handle_after_nonterminal(const item_type& item, const tree_type& node, bool) override
        {
            item;
            tree_type result(node);
            return result;
        }
};

void test_earley_parser()
{
    struct input_element
    {
        char_type* file_name;
        char_type* content;
        char_type* dot_file_name;
    };

    std::vector<input_element> inputs =
    {
        { LR"(D:\Projects\fe\grammars\Expr.G0.txt)", L"a+(a*b-(b-a)+b/a)+(a+b)", LR"(d:\tmp\fsa\Expr.G0)" },
        { LR"(D:\Projects\fe\grammars\Earley.G0.txt)", L"n+n", LR"(d:\tmp\fsa\Earley.G0)" },
        { LR"(D:\Projects\fe\grammars\Earley.G1.txt)", L"n+n", LR"(d:\tmp\fsa\Earley.G1)" },
        { LR"(D:\Projects\fe\grammars\Earley.G2.txt)", L"a*(a+a)", LR"(d:\tmp\fsa\Earley.G2)" },
        { LR"(D:\Projects\fe\grammars\Earley.G3.AYCOCK.HORSPOOL.txt)", L"a", LR"(d:\tmp\fsa\G3.AYCOCK.HORSPOOL)" },
        { LR"(D:\Projects\fe\grammars\Earley.G4.epsilon.txt)", L"aa", LR"(d:\tmp\fsa\G4.epsilon)" },
        { LR"(D:\Projects\fe\grammars\Earley.G5.cycle.txt)", L"", LR"(d:\tmp\fsa\Earley.G5.cycle)" }
    };

    uint8_t k = 1;

    for(const auto& input : inputs)
    {
        grammar gr;

        gr.load(input.file_name);

        std::wcout << grammar_visualization::decorate_grammar(gr) << std::endl;

        grammar_algorithm::build_nullability_set(gr);
        grammar_algorithm::build_first_set(gr, k);
        grammar_algorithm::build_first_set(gr, k, true);
        grammar_algorithm::build_follow_set(gr, k);
        grammar_algorithm::build_la_set(gr, k);

        using content_type = lexical_analyzer<token<earley_token_traits>>::content_type;

        content_type content(factory::create<content>());

        string_data_provider provider(input.content);

        operation_status status; //??

        bool result = (*content).load(provider, status);

        if(result)
        {
            auto lexical_analyzer(factory::create<earley_lexical_analyzer>(gr, content));

            //my_earley_parser parser(lexical_analyzer, gr, my_earley_parser::eparser_type::tree_kind::build_trees);
            my_earley_parser parser(lexical_analyzer, gr, my_earley_parser::eparser_type::tree_kind::build_ast);

            parser.parse();

            if(parser.status().custom_code() == status::custom_code::success)
            {
                auto cst(std::dynamic_pointer_cast<my_earley_parser::earley_tree>(parser.trees()[0]));

                ir<token<earley_token_traits>>::tree_type ast;

                ir<token<earley_token_traits>>::cst_to_ast(cst, ast);

                std::wcout << earley_visualization<my_earley_parser>::decorate_charts(parser.charts()).c_str() << std::endl;

                earley_visualization<my_earley_parser>::print_tree(parser.trees(), std::wcout);
                earley_visualization<my_earley_parser>::decorate_trees(parser.trees(), input.dot_file_name);
            }
            else
            {
                std::wcout << L"Failed to parse '" << input.file_name << L"'" << std::endl;
            }
        }
    }
}
