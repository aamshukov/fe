//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TAC_VISITOR_H__
#define __TAC_VISITOR_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)


template <typename Token>
struct tac_visitor : public visitor<parser_dag<Token>>
{
    using dag_type = parser_dag<Token>;

    void visit(dag_type& dag) override
    {
        dag;//??
    }
};

END_NAMESPACE

#endif // __TAC_VISITOR_H__
