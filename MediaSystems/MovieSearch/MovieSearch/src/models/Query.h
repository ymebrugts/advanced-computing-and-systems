#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file Query.h
 * @date 2025-09-17
 */

#include <string>
#include <vector>


namespace movie_search::models {
    // A structured representation of the parsed query.
    // Semantics: all vectors are ANDed; year (if present) is also ANDed.
    struct Query {
        std::vector<std::string> title_keywords; // {"Las", "Vegas"}
        bool has_year = false;
        int  year = 0;
        std::vector<std::string> genres;         // {"Drama","Romance"}
        std::vector<std::string> tags;           // {"Upton", "character study"}
    };
}
