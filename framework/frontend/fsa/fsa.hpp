//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __FSA_H__
#define __FSA_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class fsa_algorithm;

class fsa : private noncopyable
{
    public:
        using datum_type = text::datum_type;

        using token_type = fsa_state::token_type;

        using state_type = fsa_state::state_type;
        using states_type = fsa_state::states_type;

        using transition_type = fsa_state::transition_type;
        using transitions_type = fsa_state::transitions_type;

        using predicate_type = fsa_transition::predicate_type;
        using predicates_type = fsa_transition::predicates_type;

        using fsa_type = std::shared_ptr<fsa>;

    private:
        using counter_type = counter;
        
        friend class fsa_algorithm;

    private:
        state_type              my_start_state;

        states_type             my_states;
        states_type             my_final_states;

        counter_type            my_state_counter;
        counter_type            my_transition_counter;

    public:
                                fsa();
                               ~fsa();

        fsa_type                clone();

        const state_type&       start_state() const;
        state_type&             start_state();

        bool                    is_start_state(uint32_t id) const;
        bool                    is_start_state(const state_type& state) const;

        const states_type&      states() const;
        states_type&            states();

        const states_type&      final_states() const;
        states_type&            final_states();

        bool                    is_final_state(uint32_t id) const;
        bool                    is_final_state(const state_type& state) const;

        predicates_type         predicates() const;

        bool                    add_state(const state_type& state, operation_status& status);
        bool                    remove_state(uint32_t id, operation_status& status);

        bool                    add_final_state(const state_type& state, operation_status& status);

        bool                    add_transition(const state_type& start_state, const state_type& end_state, const predicate_type& predicate, operation_status& status);
        bool                    add_transition(const state_type& start_state, const state_type& end_state, datum_type predicate, operation_status& status);

        static bool             combine(const fsa_type& fsa1, const fsa_type& fsa2, fsa_type& result_fsa, operation_status& status);
        static bool             combine(const std::vector<fsa_type>& fsas, fsa_type& result_fsa, operation_status& status);

        static bool             concatenate(const fsa_type& fsa1, const fsa_type& fsa2, fsa_type& result_fsa, operation_status& status);
};

inline const typename fsa::state_type& fsa::start_state() const
{
    return my_start_state;
}

inline typename fsa::state_type& fsa::start_state()
{
    return const_cast<state_type&>(static_cast<const fsa&>(*this).start_state());
}

inline bool fsa::is_start_state(uint32_t id) const
{
    return (*my_start_state).id() == id;
}

inline bool fsa::is_start_state(const typename fsa::state_type& state) const
{
    return is_start_state((*state).id());
}

inline const typename fsa::states_type& fsa::states() const
{
    return my_states;
}

inline typename fsa::states_type& fsa::states()
{
    return const_cast<states_type&>(static_cast<const fsa&>(*this).states());
}

inline const typename fsa::states_type& fsa::final_states() const
{
    return my_final_states;
}

inline typename fsa::states_type& fsa::final_states()
{
    return const_cast<states_type&>(static_cast<const fsa&>(*this).final_states());
}

inline bool fsa::is_final_state(uint32_t id) const
{
    return final_states().find(id) != final_states().end();
}

inline bool fsa::is_final_state(const typename fsa::state_type& state) const
{
    return is_final_state((*state).id());
}

END_NAMESPACE

#endif // __FSA_H__
