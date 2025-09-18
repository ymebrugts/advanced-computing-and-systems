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

    std::vector<movie_parser::models::Movie> search_movies(
        const movie_search::models::Query& query,
        const std::vector<movie_parser::models::Movie>& movies,
        const std::vector<movie_parser::models::MovieTag>& tags
    ) {
        std::vector<movie_parser::models::Movie> results;

        for (const auto& movie : movies) {
            bool match = true;

            // All title keywords must appear and case insensitive
            for (const auto& keyword : query.title_keywords) {
                if (!shared::utils::insensitive_contains_word(movie.title, keyword)) {
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
                for (const auto& genre : query.genres) {
                    if (!shared::utils::insensitive_contains_word(movie.genres, genre)) {
                        match = false;
                        break;
                    }
                }
            }

            // Tags. At least one must match, across all movie tags.
            // Since human made it uses contains instead of matching on the whole word
            if (match && !query.tags.empty()) {
                bool found = false;
                for (const auto& tag : tags) {
                    if (tag.movie_id == movie.movie_id) {
                        for (const auto& query_tag : query.tags) {
                            if (shared::utils::insensitive_contains_word(tag.tag, query_tag)) {
                                found = true;
                                break;
                            }
                        }
                    }
                    if (found) break;
                }
                if (!found) match = false;
            }

            if (match) {
                results.push_back(movie);
            }
        }

        return results;
    }

}
