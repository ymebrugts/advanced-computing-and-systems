#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file tags_parser.h
 * @date 2025-09-17
 */

#include <string>
#include <vector>
#include "../models/MovieTag.h"

namespace movie_parser::parsers {

    /**
     * @brief Load tags from a MovieLens tags.dat file.
     * @param filename Path to tags.dat
     * @return Vector of Tag structs
     */
    std::vector<movie_parser::models::MovieTag> load_tags(const std::string& filename);

}
