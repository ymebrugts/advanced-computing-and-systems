/**
 * author Yme Brugts (s4536622)
 * @file search_service.cpp
 * @date 2025-09-17
 */

#include "search_service.h"
#include <algorithm>
#include <cctype>
#include <optional>

namespace {

    // Case-insensitive substring search
    bool icontains(const std::string& text, const std::string& word) {
        auto it = std::search(
            text.begin(), text.end(),
            word.begin(), word.end(),
            [](unsigned char ch1, unsigned char ch2) {
                return std::tolower(ch1) == std::tolower(ch2);
            }
        );
        return it != text.end();
    }

    // Extract year from "Title (YYYY)"
    std::optional<int> extractYear(const std::string& title) {
        auto pos1 = title.find_last_of('(');
        auto pos2 = title.find_last_of(')');
        if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1 + 1) {
            try {
                return std::stoi(title.substr(pos1 + 1, pos2 - pos1 - 1));
            }
            catch (...) {}
        }
        return std::nullopt;
    }

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
                if (!icontains(m.title, kw)) {
                    match = false;
                    break;
                }
            }

            // Year filter
            if (match && q.has_year) {
                auto y = extractYear(m.title);
                if (!y || *y != q.year) match = false;
            }

            // All genres must appear
            if (match && !q.genres.empty()) {
                for (const auto& g : q.genres) {
                    if (!icontains(m.genres, g)) {
                        match = false;
                        break;
                    }
                }
            }

            // Tags. At least one must match, across all movie tags.
            if (match && !q.tags.empty()) {
                bool found = false;
                for (const auto& t : tags) {
                    if (t.movie_id == m.movie_id) {
                        for (const auto& qtag : q.tags) {
                            if (icontains(t.tag, qtag)) {
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
