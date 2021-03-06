//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core\pch.hpp>
#include <core\noncopyable.hpp>
#include <core\status.hpp>
#include <core\text.hpp>
#include <core\domain_helper.hpp>
#include <core\logger.hpp>
#include <core\counter.hpp>

#include <frontend\fsa\fsa_transition.hpp>
#include <frontend\fsa\fsa_state.hpp>
#include <frontend\fsa\fsa.hpp>
#include <frontend\fsa\fsa_codegen.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

string_type fsa_codegen::build_predicate(const string_type& predicate)
{
    string_type result;

    result += L"L'";
    result += predicate;
    result += L"'";

    return result;
}

END_NAMESPACE
