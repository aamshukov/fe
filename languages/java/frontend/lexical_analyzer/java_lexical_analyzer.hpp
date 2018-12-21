//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __JAVA_LEXICAL_ANALYZER_H__
#define __JAVA_LEXICAL_ANALYZER_H__

#pragma once

BEGIN_NAMESPACE(java)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class java_lexical_analyzer : public lexical_analyzer<token<java_token_traits>>
{
    public:
        using fsa_type = fsa::fsa_type;
        using lexical_analyzer_type = std::shared_ptr<java_lexical_analyzer>;

        using line_map_type = std::unique_ptr<loc_t[]>;
        using tab_map_type = std::vector<bool>;

    private:
        bool                    my_unicode;         // true if the last obtained codepoint from unicode-escape
        uint8_t                 my_unicode_length;
        uint32_t                my_unicode_backslash_count;

        line_map_type           my_line_map;        // start position of each line
        uint32_t                my_line_map_size;
        loc_t                   my_cached_line;
        loc_t                   my_cached_line_position;

        tab_map_type            my_tab_map;         // tab positions
        uint8_t                 my_tab_size;        // tab size, default is 4

    private:
        static void             build_fsa_from_re(const string_type& re,
                                                  token_type::traits::type token,
                                                  token_type::traits::type escape_token,
                                                  const string_type& escape_predicate,
                                                  fsa_type& result_fsa);

        datum_type              consume_unicode_escape(bool check_for_surrogates = true);

        datum_type              advance();
        datum_type              peek(uint16_t k = 1);
        void                    rewind();

        void                    build_line_map();
        loc_t                   find_line_number(loc_t position);

    protected:
        virtual void            next_lexeme_impl() override;

    public:
        static void             build_numeric_literals(const string_type& file_name, const string_type& label_prefix);
        static void             build_identifiers_and_keywords(const string_type& file_name, const string_type& label_prefix);

    public:
                                java_lexical_analyzer(const content_type& content, uint8_t tab_size);
                               ~java_lexical_analyzer();

        static bool             create(const string_type& file_name,
                                       lexical_analyzer_type& result_lexical_analyzer,
                                       operation_status& status,
                                       uint8_t tab_size = 4);

        loc_t                   get_line_number(loc_t position);
        loc_t                   get_column_number(loc_t position);
};

END_NAMESPACE

#endif // __JAVA_LEXICAL_ANALYZER_H__
