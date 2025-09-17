#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file tags_parser.h
 * @date 2025-09-17
 */

#include <string>
#include <vector>
#include "../models/movie_tag.h"

namespace movie_parser::parsers {

    /**
     * @brief Load tags from a MovieLens tags.dat file.
     * @param filename Path to tags.dat
     * @return Vector of Tag structs
     */
    std::vector<movie_parser::models::movie_tag> loadTags(const std::string& filename);

}
