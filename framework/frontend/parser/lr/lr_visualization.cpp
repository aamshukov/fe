﻿//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\domain_helper.hpp>
#include <core\flags.hpp>
#include <core\text.hpp>
#include <core\enumerate.hpp>
#include <core\counter.hpp>

#include <frontend\grammar\symbol.hpp>
#include <frontend\grammar\rule.hpp>
#include <frontend\grammar\grammar.hpp>
#include <frontend\grammar\grammar_algorithm.hpp>
#include <frontend\grammar\grammar_visualization.hpp>

#include <frontend\parser\lr\lr_algorithm.hpp>
#include <frontend\parser\lr\lr_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

string_type lr_visualization::decorate_lr_item(const typename lr_visualization::lr_item_type& item)
{
    string_type result;

    result += L"[ ";

    for(const auto& sym : (*(*item).rule).lhs())
    {
        if((*sym).terminal() && (*sym).id() != (*symbol::epsilon).id() && (*sym).id() != (*symbol::op_mark).id())
        {
            result += L"'";
        }

        result += (*sym).name();

        if((*sym).terminal() && (*sym).id() != (*symbol::epsilon).id() && (*sym).id() != (*symbol::op_mark).id())
        {
            result += L"'";
        }

        result += L" ";
    }

    result += L": ";

    for(const auto& [k, sym] : enumerate((*(*item).rule).rhs()))
    {
        if(k == (*item).dot)
        {
            result += L". ";
        }

        if((*sym).terminal() && (*sym).id() != (*symbol::epsilon).id() && (*sym).id() != (*symbol::op_mark).id())
        {
            result += L"'";
        }

        result += (*sym).name();

        if((*sym).terminal() && (*sym).id() != (*symbol::epsilon).id() && (*sym).id() != (*symbol::op_mark).id())
        {
            result += L"'";
        }

        result += L" ";
    }

    if((*item).dot == (*(*item).rule).rhs().size())
    {
        result += L". , ";
    }
    else
    {
        result += L", ";
    }

    result += grammar_visualization::decorate_sets((*item).la, false);

    result += L"]";
    result += L"\n";

    return result;
}

string_type lr_visualization::decorate_lr_items(const typename lr_visualization::lr_items_type& items)
{
    string_type result;

    std::wstringstream content;

    std::for_each(items.begin(),
                  items.end(),
                  [&content](const auto& item)
                  {
                      content << decorate_lr_item(item).c_str();
                  });

    result = content.str();

    return result;
}

string_type lr_visualization::decorate_lr_transition(const typename lr_visualization::lr_transition_type& transition)
{
    string_type result;

    result += L"on " + (*(*transition).symbol).name() + L" -> ";

    char_type buffer[64];
    snwprintf(buffer, array_size(buffer), TRUNCATE, L"[%d]\n", (*(*transition).state).id);

    result += buffer;

    return result;
}

string_type lr_visualization::decorate_lr_transitions(const typename lr_visualization::lr_transitions_type& transitions)
{
    string_type result;

    std::wstringstream content;

    std::for_each(transitions.begin(),
                  transitions.end(),
                  [&content](const auto& transition_kvp)
                  {
                      content << decorate_lr_transition(transition_kvp.second).c_str();
                  });

    result = content.str();

    return result;
}

string_type lr_visualization::decorate_lr_state(const lr_state_type& state)
{
    string_type result;

    char_type buffer[64];
    snwprintf(buffer, array_size(buffer), TRUNCATE, L"[%4d (%zd) %s] ", (*state).id, (*state).items.size(), (((*state).flags & lr_algorithm::flags::final) == lr_algorithm::flags::final) ? L"*" : L"");

    result += buffer;
    result += L"\n";

    result += decorate_lr_items((*state).items);
    result += decorate_lr_transitions((*state).transitions);

    result += L"\n";

    return result;
}

string_type lr_visualization::decorate_lr_states(const typename lr_visualization::lr_states_type& states)
{
    string_type result;

    std::wstringstream content;

    std::for_each(states.begin(),
                  states.end(),
                  [&content](const auto& state)
                  {
                      content << decorate_lr_state(state).c_str();
                  });

    result = content.str();

    return result;
}

string_type lr_visualization::decorate_lr_goto_table(const grammar& gr,
                                                     const typename lr_visualization::lr_states_type& states,
                                                     const typename lr_visualization::lr_goto_table_type& goto_table)
{
    string_type result;

    const std::size_t table_cell_width = 8;

    auto alignment = std::left;

    std::wstringstream content;

    content <<
    std::endl <<
    alignment << std::setw(table_cell_width) << std::setfill(L' ') << L' ';

    // collect nonterminals
    std::vector<symbol_type> nonterminals;

    grammar_algorithm::collect_nonterminals(gr, nonterminals);


    for(const auto& nonterminal : nonterminals)
    {
        content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << (*nonterminal).name().c_str();
    }

    content << std::endl;

    for(const auto& _ : nonterminals)
    {
        _;
        content << alignment << std::setw(table_cell_width + 1) << std::setfill(L'-') << L"-";
    }

    content << std::endl;

    for(std::size_t i = 0, n = states.size(); i < n; i++) // i = state id
    {
        content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << std::to_wstring(i);

        for(const auto& nonterminal : nonterminals)
        {
            string_type entry_text = L" ";

            const auto& key(std::make_pair((*nonterminal).id(), static_cast<uint32_t>(i)));

            const auto it(goto_table.find(key));
            
            if(it != goto_table.end())
            {
                entry_text = std::to_wstring((*it).second);
            }

            content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << entry_text.c_str();
        }

        content << std::endl;
    }

    result = content.str();

    return result;
}

string_type lr_visualization::decorate_lr_action_table(uint8_t k,
                                                       const typename lr_visualization::sets_type& la_set,
                                                       const typename lr_visualization::lr_states_type& states,
                                                       const typename lr_visualization::lr_action_table_type& action_table)
{
    string_type result;

    const std::size_t table_cell_width = 12 * k;

    auto alignment = std::left;

    std::wstringstream content;

    content <<
    std::endl <<
    alignment << std::setw(table_cell_width) << std::setfill(L' ') << L' ';

    for(const auto& la : la_set)
    {
        content <<
        alignment <<
        std::setw(table_cell_width) <<
        std::setfill(L' ') <<
        text::trim(grammar_visualization::decorate_set(la, false), L" ").c_str();
    }

    content << std::endl;

    for(const auto& _ : la_set)
    {
        _;
        content << alignment << std::setw(table_cell_width + 1) << std::setfill(L'-') << L"-";
    }

    content << std::endl;

    for(std::size_t i = 0, n = states.size(); i < n; i++) // i = state id
    {
        content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << std::to_wstring(i);

        for(const auto& la : la_set)
        {
            string_type entry_text;

            std::vector<uint32_t> func_la; // f(u)

            std::for_each(la.begin(), la.end(), [&func_la](const auto& symb){ func_la.emplace_back((*symb).id()); });

            auto key(std::make_pair(func_la, static_cast<uint32_t>(i)));

            auto it(action_table.find(key));
            
            if(it != action_table.end())
            {
                const auto& entry((*it).second);

                std::for_each(entry.begin(),
                              entry.end(),
                              [&entry_text, i](const auto& e)
                              {
                                  if(lr_algorithm::lr_action((int)e) == lr_algorithm::lr_action::accept)
                                  {
                                      entry_text += L"a";
                                  }
                                  else if((int32_t)e < 0)
                                  {
                                      entry_text += L"s" + std::to_wstring(-(int32_t)e);
                                  }
                                  else
                                  {
                                      entry_text += L"r" + std::to_wstring(e);
                                  }
                                  entry_text += L',';
                              });

                entry_text = text::trim(entry_text, L",");
            }

            content << alignment << std::setw(table_cell_width) << std::setfill(L' ') << entry_text.c_str();
        }

        content << std::endl;
    }

    result = content.str();

    return result;
}

END_NAMESPACE
