//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_TREE_H__
#define __PARSER_TREE_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)

template <typename Token, typename TreeKind>
struct parser_tree : public tree, public visitable<parser_tree<Token, TreeKind>>
{
    using token_type = Token;
    using tree_kind_type = TreeKind;

    using symbol_type = grammar::symbol_type;

    using record_type = typename symbol_table_record<token_type>::record_type;
    using records_type = typename symbol_table_record<token_type>::records_type;

    using visitor_type = visitor<parser_tree<token_type, tree_kind_type>>;

    symbol_type symbol;
    record_type record;

    virtual ~parser_tree()
    {
    }

    void accept(visitor_type& visitor) override
    {
        visitor.visit(static_cast<parser_tree<token_type, tree_kind_type>&>(*this));
    }
};

END_NAMESPACE

#endif // __PARSER_TREE_H__
