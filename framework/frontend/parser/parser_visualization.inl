﻿//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_VISUALIZATION_INL__
#define __PARSER_VISUALIZATION_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename T>
void parser_visualization<T>::print_tree(const typename parser_visualization<T>::tree_type& tree, std::size_t level, std::wostream& stream)
{
    for(std::size_t i = 0; i < level; i++)
    {
        stream << "    ";
    }

    if(static_cast<uint32_t>((*tree).token.type) != 0)
        stream << (*(*tree).symbol).name() << L":" << static_cast<uint32_t>((*tree).token.type) << std::endl;
    else
        stream << (*(*tree).symbol).name() << std::endl;

    for(const auto& kid : (*tree).kids)
    {
        print_tree(std::dynamic_pointer_cast<parser_tree<token_type>>(kid), level + 1, stream);
    }

    stream << std::endl;
}

template <typename T>
void parser_visualization<T>::print_trees(const typename parser_visualization<T>::trees_type& trees, std::wostream& stream)
{
    stream << std::endl;

    for(auto [k, tree] : enumerate(trees))
    {
        stream << L"[" << k << L"]:" << std::endl;
        print_tree(tree, 0, stream);
    }

    stream << std::endl;
}

template <typename T>
template <typename U, typename S>
void parser_visualization<T>::collect_dot_labels(const U& root, std::wostream& stream)
{
    std::size_t k = 1;

    std::queue<U> queue;

    queue.emplace(root);

    while(!queue.empty())
    {
        auto entry(queue.front());

        queue.pop();

        stream << L"    " << k++ << L" [label=\"" << (*(*entry).symbol).name() << L"\"];" << std::endl;

        for(auto kid : (*entry).kids)
        {
            queue.emplace(std::dynamic_pointer_cast<S>(kid));
        }
    }

    stream << std::endl;
}

template <typename T>
void parser_visualization<T>::build_tree_dot_graph(const typename parser_visualization<T>::tree_type& tree, std::wostream& stream)
{
    struct queue_entry
    {
        tree_type node;
        std::size_t papa;
    };

    std::size_t k = 1;

    std::queue<queue_entry> queue;

    queue.emplace(queue_entry{ tree, k });

    while(!queue.empty())
    {
        auto entry(queue.front());

        queue.pop();

        for(auto kid : (*entry.node).kids)
        {
            stream << L"    " << entry.papa << L" -> " << ++k << L";" << std::endl;
            queue.emplace(queue_entry{ std::dynamic_pointer_cast<parser_tree<token_type>>(kid), k });
        }
    }
}

template <typename T>
void parser_visualization<T>::build_dag_dot_graph(const typename parser_visualization<T>::dag_type& dag, std::wostream& stream)
{
    struct queue_entry
    {
        dag_type node;
        std::size_t papa;
    };

    std::size_t k = 1;

    std::queue<queue_entry> queue;

    queue.emplace(queue_entry{ dag, k });

    while(!queue.empty())
    {
        auto entry(queue.front());

        queue.pop();

        //??

        for(auto kid : (*entry.node).kids)
        {
            stream << L"    " << entry.papa << L" -> " << ++k << L";" << std::endl;
            queue.emplace(queue_entry{ std::dynamic_pointer_cast<parser_dag<token_type>>(kid), k });
        }
    }
}

template <typename T>
void parser_visualization<T>::decorate_tree(const typename parser_visualization<T>::tree_type& tree, const string_type& dot_file_name, std::size_t k)
{
    string_type file_name(dot_file_name + std::to_wstring(k) + L".etree.dot");

    std::wofstream stream;

    try
    {
        stream.open(file_name.c_str(), std::ios::out | std::ios::binary);

        if(!stream.is_open() || stream.bad() || stream.fail())
        {
            log_error(L"Failed to generate graphviz file %s: stream is either open or in a bad condition.", file_name.c_str());
        }

        stream << L"digraph EarleyTree" << std::endl;
        stream << L"{" << std::endl;

        collect_dot_labels<tree_type, parser_tree<token_type>>(tree, stream);
        build_tree_dot_graph(tree, stream);

        stream << L"}" << std::endl;

        stream.flush();
        stream.clear();
        stream.close();
    }
    catch(const std::exception& ex)
    {
        log_exception(ex, L"Failed to generate graphviz file %s: error occurred.", file_name.c_str());
    }
}

template <typename T>
void parser_visualization<T>::decorate_trees(const typename parser_visualization<T>::trees_type& trees, const string_type& dot_file_name)
{
    // generate Graphviz dot file ...
    // for %i in (d:\tmp\fsa\*.dot) do D:\Soft\graphviz\2.38\release\bin\dot -Tpng %i -o %i.png
    for(auto [k, tree] : enumerate(trees))
    {
        decorate_tree(tree, dot_file_name, k);
    }
}

template <typename T>
void parser_visualization<T>::decorate_dag(const typename parser_visualization<T>::dag_type& dag, const string_type& dot_file_name)
{
    string_type file_name(dot_file_name + L".dag.dot");

    std::wofstream stream;

    try
    {
        stream.open(file_name.c_str(), std::ios::out | std::ios::binary);

        if(!stream.is_open() || stream.bad() || stream.fail())
        {
            log_error(L"Failed to generate graphviz file %s: stream is either open or in a bad condition.", file_name.c_str());
        }

        stream << L"digraph DAG" << std::endl;
        stream << L"{" << std::endl;

        collect_dot_labels<dag_type, parser_dag<token_type>>(dag, stream);
        build_dag_dot_graph(dag, stream);

        stream << L"}" << std::endl;

        stream.flush();
        stream.clear();
        stream.close();
    }
    catch(const std::exception& ex)
    {
        log_exception(ex, L"Failed to generate graphviz file %s: error occurred.", file_name.c_str());
    }
}

END_NAMESPACE

#endif // __parser_visualization_INL__
