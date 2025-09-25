#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file Movie.h
 * @date 2025-09-17
 */

#include <optional>
#include <string>


namespace movie_parser::models {
    struct Movie {
        int movie_id;
        std::string title;
        std::vector<std::string> genres;
        std::optional<int> year;
    };
}
