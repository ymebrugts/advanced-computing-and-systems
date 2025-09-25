#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file command_service.h
 * @date 2025-09-16
 */


#include <string>
#include <vector>
#include <ostream>
#include "../models/ParseResult.h"

namespace moviesearch::services {
    // parse a raw input line that starts with "moviesearch".
    movie_search::models::ParseResult parse_moviesearch_line(const std::vector<std::string>& arguments);
}
