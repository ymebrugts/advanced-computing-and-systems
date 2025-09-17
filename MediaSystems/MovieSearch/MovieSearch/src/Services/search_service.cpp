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

    std::vector<movie_parser::models::Movie> searchMovies(
        const movie_search::models::Query& q,
        const std::vector<movie_parser::models::Movie>& movies,
        const std::vector<movie_parser::models::MovieTag>& tags
    ) {
        std::vector<movie_parser::models::Movie> results;

        for (const auto& m : movies) {
            bool match = true;

            // All title keywords must appear and case insensitive
            for (const auto& kw : q.title_keywords) {
                if (!shared::utils::insensitive_contains_word(m.title, kw)) {
                    match = false;
                    break;
                }
            }

            // Year filter
            if (match && q.has_year) {
                if (!m.year || *m.year != q.year) {
                	match = false;
                }
            }

            // All genres must appear
            if (match && !q.genres.empty()) {
                for (const auto& g : q.genres) {
                    if (!shared::utils::insensitive_contains_word(m.genres, g)) {
                        match = false;
                        break;
                    }
                }
            }

            // Tags. At least one must match, across all movie tags.
            // Since human made it uses contains instead of matching on the whole word
            if (match && !q.tags.empty()) {
                bool found = false;
                for (const auto& t : tags) {
                    if (t.movie_id == m.movie_id) {
                        for (const auto& qtag : q.tags) {
                            if (shared::utils::insensitive_contains(t.tag, qtag)) {
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
                results.push_back(m);
            }
        }

        return results;
    }

}
