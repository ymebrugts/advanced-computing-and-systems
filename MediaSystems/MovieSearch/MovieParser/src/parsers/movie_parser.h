#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file movie_parser.h
 * @date 2025-09-17
 */

#pragma once

#include <string>
#include <vector>
#include "../models/movie.h"

namespace movie_parser::parsers {

    /**
     * @brief Load movies from a MovieLens movies.dat file.
     * @param filename Path to movies.dat
     * @return Vector of Movie structs
     */
    std::vector<movie_parser::models::Movie> loadMovies(const std::string& filename);

}