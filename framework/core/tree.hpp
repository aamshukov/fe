//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TREE_H__
#define __TREE_H__

#pragma once

BEGIN_NAMESPACE(core)

struct tree
{
    enum class flags : uint64_t
    {
        clear       = 0x0000,
        root_in_ast = 0x0002, // ANTLR, expr : mexpr (�+�^ mexpr)* EOF! ;
        not_in_ast  = 0x0004  // ANTLR, expr : mexpr (�+�^ mexpr)* EOF! ;
    };

    using flags_type = tmpl_flags<flags>;

    using tree_type = std::shared_ptr<tree>;
    using trees_type = std::vector<tree_type>;

    tree_type papa; // parent
    trees_type kids;

    flags_type flags;

    virtual ~tree()
    {
    }
};

END_NAMESPACE

#endif // __TREE_H__
