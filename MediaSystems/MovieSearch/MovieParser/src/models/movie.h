#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file Movie.h
 * @date 2025-09-17
 */

#include <string>


namespace movie_parser::models {
    struct Movie {
        int movie_id;
        std::string title;
        std::string genres;  // or split into vector<string>
    };
}
