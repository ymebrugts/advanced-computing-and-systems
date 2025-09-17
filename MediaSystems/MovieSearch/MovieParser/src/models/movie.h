#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file movie.h
 * @date 2025-09-17
 */

#include <string>


namespace movie_parser::models {
    struct movie {
        int movieId;
        std::string title;
        std::string genres;  // or split into vector<string>
    };
}
