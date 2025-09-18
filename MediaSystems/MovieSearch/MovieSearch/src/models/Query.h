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
    struct Query {
        std::vector<std::string> title_keywords;
        bool has_year = false;
        int  year = 0;
        std::vector<std::string> genres;
        std::vector<std::string> tags;
    };
}
