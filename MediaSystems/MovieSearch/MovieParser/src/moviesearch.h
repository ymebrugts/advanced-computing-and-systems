#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file moviesearch.h
 * @date 2025-09-16
 */


#include <string>
#include <vector>
#include <ostream>

namespace moviesearch {

    // A structured representation of the parsed query.
    // Semantics: all vectors are ANDed; year (if present) is also ANDed.
    struct Query {
        std::vector<std::string> title_keywords; // {"Las", "Vegas"}
        bool has_year = false;
        int  year = 0;
        std::vector<std::string> genres;         // {"Drama","Romance"}
        std::vector<std::string> tags;           // {"Upton", "character study"}
    };

    // Result object with errors/warnings; ok == errors.empty()
    struct ParseResult {
        bool ok = false;
        Query query;
        std::vector<std::string> errors;
        std::vector<std::string> warnings;
    };

    // Quoting-aware tokenizer for a single command line (normalizes Unicode dashes to '-').
    std::vector<std::string> tokenize_command_line(const std::string& line);

    // Parse tokens that appear AFTER the leading "moviesearch" token.
    // Recognized options: --title, --year, --genre, --tag 
    // Supports --opt value..., --opt=value, repeated flags, comma-separated lists
    // and multi-word values (title keywords split on whitespace unless quoted).
    ParseResult parse_moviesearch_tokens(const std::vector<std::string>& args);

    // Convenience: parse a raw input line that starts with "moviesearch ...".
    ParseResult parse_moviesearch_line(const std::string& line);

    // Human-readable representation (for debugging / runner output).
    void print_query(std::ostream& out, const Query& q);

}
