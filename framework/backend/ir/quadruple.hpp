//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __QUADRUPLE_H__
#define __QUADRUPLE_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

struct quadruple
{
    using symbol_type = symbol::symbol_type;
    using symbols_type = symbol::symbols_type;

    symbol_type operation;
    symbol_type argument1;
    symbol_type argument2;
    symbol_type result;

    quadruple()
    {
    }

    quadruple(const symbol_type& operation,
              const symbol_type& argument1,
              const symbol_type& argument2,
              const symbol_type& result)
        : operation(operation), argument1(argument1), argument2(argument2), result(result)
    {
    }

    quadruple(const symbol_type& operation,
              const symbol_type& argument1,
              const symbol_type& result)
        : quadruple(operation, argument1, nullptr, result)
    {
    }

    quadruple(const symbol_type& operation,
              const symbol_type& result)
        : quadruple(operation, nullptr, nullptr, result)
    {
    }

    quadruple(const symbol_type& operation)
        : quadruple(operation, nullptr, nullptr, nullptr)
    {
    }

    quadruple(const quadruple& other)
    {
        if(this != &other)
        {
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }
    }

    quadruple(quadruple&& other)
    {
        if(this != &other)
        {
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }
    }

    const quadruple& operator = (const quadruple& other)
    {
        if(this != &other)
        {
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }

        return *this;
    }

    quadruple& operator = (quadruple&& other)
    {
        if(this != &other)
        {
            operation = other.operation;
            argument1 = other.argument1;
            argument2 = other.argument2;
            result    = other.result;
        }

        return *this;
    }
};

END_NAMESPACE

#endif // __QUADRUPLE_H__
