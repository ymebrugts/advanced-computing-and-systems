#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file rating_parser.h
 * @date 2025-09-17
 */

#include <string>
#include <vector>
#include "../models/movie_rating.h"

namespace movie_parser::parsers {

    /**
     * @brief Load ratings from a MovieLens ratings.dat file.
     * @param filename Path to ratings.dat
     * @return Vector of Rating structs
     */
    std::vector<movie_parser::models::movie_rating> loadRatings(const std::string& filename);

}
