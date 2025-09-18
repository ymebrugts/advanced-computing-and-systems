#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file command_service.h
 * @date 2025-09-16
 */


#include <string>
#include <vector>
#include <ostream>
#include "../models/ParseResult.h"

namespace moviesearch::services {

    // Quoting-aware tokenizer for a single command line (normalizes Unicode dashes to '-').
    std::vector<std::string> tokenize_command_line(const std::string& line);

    // Parse tokens that appear AFTER the leading "moviesearch" token.
    // Recognized options: --title, --year, --genre, --tag 
    // Supports --opt value..., --opt=value, repeated flags, comma-separated lists
    // and multi-word values (title keywords split on whitespace unless quoted).
    movie_search::models::ParseResult parse_moviesearch_tokens(const std::vector<std::string>& args);

    // Convenience: parse a raw input line that starts with "moviesearch ...".
    movie_search::models::ParseResult parse_moviesearch_line(const std::string& line);
}
