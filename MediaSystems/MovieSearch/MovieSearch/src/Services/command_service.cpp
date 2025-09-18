/**
 * author Yme Brugts (s4536622)
 * @file command_service.cpp
 * @date 2025-01-16
 */

#include "command_service.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

#include "cmdline_utils.h"
#include "container_utils.h"

#include "../models/ParseResult.h"
#include "../models/Query.h"

namespace moviesearch::services {
	namespace {
        void process_moviesearch_param(std::vector<std::string>& parsed_values,
            const std::string& command_name,
            const std::vector<std::string>& args,
            std::size_t&  number_of_arguments,
            movie_search::models::ParseResult& parse_result) {
            auto vals = shared::utils::collect_value_tokens(args, number_of_arguments);
            if (vals.empty()) {
                parse_result.errors.push_back("Missing value for --" + command_name);
                return;
            }
            parsed_values.insert(parsed_values.end(), vals.begin(), vals.end());
        }

	    void handle_year_option(movie_search::models::Query& query, const std::vector<std::string>& args, std::size_t& i, movie_search::models::ParseResult& parse_result) {
	        auto vals = shared::utils::collect_value_tokens(args, i);
	        if (vals.empty()) {
	            parse_result.errors.emplace_back("Missing value for --year");
	            return;
	        }
	        if (vals.size() > 1) {
	            parse_result.errors.emplace_back("Too many values for --year (expected one)");
	            return;
	        }
	        try {
	            query.year = std::stoi(vals.front());
	            query.has_year = true;
	        }
	        catch (...) {
	            parse_result.errors.push_back("Invalid year: '" + vals.front() + "'");
	        }
	    }

	    void parse_tokenized_args_into_query(const std::vector<std::string>& tokenized_args, movie_search::models::Query& query, movie_search::models::ParseResult& parse_result) {
	        std::size_t i = 0;
	        while (i < tokenized_args.size()) {
	            const std::string& token = tokenized_args[i++];

	            if (!shared::utils::token_is_option(token)) {
	                parse_result.warnings.push_back("Ignoring unexpected token: '" + token + "'");
	                continue;
	            }

	            if (shared::utils::matches_option(token, "title")) {
	                process_moviesearch_param(query.title_keywords, "title", tokenized_args, i, parse_result);
	            }
	            else if (shared::utils::matches_option(token, "year")) {
	                handle_year_option(query, tokenized_args, i, parse_result);
	            }
	            else if (shared::utils::matches_option(token, "genre") || shared::utils::matches_option(token, "genres")) {
	                process_moviesearch_param(query.genres, "genre", tokenized_args, i, parse_result);
	            }
	            else if (shared::utils::matches_option(token, "tag") || shared::utils::matches_option(token, "tags")) {
	                process_moviesearch_param(query.tags, "tag", tokenized_args, i, parse_result);
	            }
	            else {
	                parse_result.errors.push_back("Unknown option: '" + token + "'");
	                while (i < tokenized_args.size() && !shared::utils::token_is_option(tokenized_args[i])) ++i; // skip
	            }
	        }
	    }

	    void finalize_result(movie_search::models::Query& query, movie_search::models::ParseResult& parse_result) {
	        // Require at least one filter
	        if (query.title_keywords.empty() && !query.has_year && query.genres.empty() && query.tags.empty()) {
	            parse_result.errors.push_back("moviesearch requires at least one filter (--title/--year/--genre/--tag)");
	        }

	        // Dedupe lists while preserving order
	        shared::utils::dedupe_preserve_order(query.title_keywords);
	        shared::utils::dedupe_preserve_order(query.genres);
	        shared::utils::dedupe_preserve_order(query.tags);

	        parse_result.ok = parse_result.errors.empty();
	    }
	}

	std::vector<std::string> tokenize_command_line(const std::string& terminal_input) {
		std::vector<std::string> tokens;
		std::string current_token;

		for (char input_char : terminal_input) {
			if (std::isspace(static_cast<unsigned char>(input_char))) {
				if (!current_token.empty()) {
					tokens.emplace_back(std::move(current_token));
					current_token.clear();
				}
			}
			else {
				current_token.push_back(input_char);
			}
		}

		if (!current_token.empty()) {
			tokens.emplace_back(std::move(current_token));
		}

		return tokens;
	}

	movie_search::models::ParseResult parse_moviesearch_tokens(const std::vector<std::string>& arguments) {
		movie_search::models::ParseResult parse_result;
		movie_search::models::Query& query = parse_result.query;

	    parse_tokenized_args_into_query(arguments, query, parse_result);
	    finalize_result(query, parse_result);

	    return parse_result;
	}   

}