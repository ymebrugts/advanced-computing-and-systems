/**
 * author Yme Brugts (s4536622)
 * @file search_service.cpp
 * @date 2025-09-17
 */

#include "search_service.h"
#include <algorithm>
#include <cctype>
#include <optional>

#include "string_utils.h"

namespace {

}

namespace movie_search::services {

    bool check_match_list(const std::vector<std::string>& queried_list, const std::vector<std::string>& genres) {
        for (const auto& query : queried_list) {
            bool found = false;
            for (const auto& parsed_token : genres) {
                if (shared::utils::case_insensitive_contains_word(parsed_token, query))
                {
                    found = true; break;
                }
            }
            if (!found) return false; // one didn't match
        }
        return true; // all queries matched
    }

    bool check_match_tags(const movie_parser::models::Movie& movie,
        const std::vector<std::string>& queried_list,
        const std::vector<movie_parser::models::MovieTag>& tags) {
        for (const auto& query : queried_list) {
            bool found = false;
            for (const auto& tag : tags) {
                if (tag.movie_id == movie.movie_id &&
                    shared::utils::case_insensitive_contains_word(tag.tag, query)) {
                    found = true; break;
                }
            }
            if (!found) return false; // one didn't match
        }
        return true; // all queries matched
    }

    std::vector<movie_parser::models::Movie> search_movies(
        const movie_search::models::Query& query,
        const std::vector<movie_parser::models::Movie>& movies,
        const std::vector<movie_parser::models::MovieTag>& tags
    ) {
        std::vector<movie_parser::models::Movie> results;

        for (const auto& movie : movies) {
            bool match = true;

            // All title keywords must appear and case insensitive
            for (const auto& keyword : query.titles) {
                if (!shared::utils::case_insensitive_contains_word(movie.title, keyword)) {
                    match = false;
                    break;
                }
            }

            // Year filter
            if (match && query.has_year) {
                if (!movie.year || *movie.year != query.year) {
                	match = false;
                }
            }

            // All genres must appear
            if (match && !query.genres.empty()) {

                if (!check_match_list(query.genres, movie.genres))
                {
                    match = false;
                }
            }

            // Tags. All must match, across all movie tags.
            // Since human made it uses contains instead of matching on the whole word
            if (match && !query.tags.empty()) {

                if (!check_match_tags(movie, query.tags, tags))
                {
                    match = false;
                }
            }


            if (match) {
                results.push_back(movie);
            }
        }

        return results;
    }
}
