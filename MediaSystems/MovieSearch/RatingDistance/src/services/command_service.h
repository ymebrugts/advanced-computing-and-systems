#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file command_service.h
 * @date 2025-09-23
 */


#include <string>
#include <vector>
#include <ostream>

#include "../models/Query.h"

namespace rating_distance::services {
    // parse a raw input line that starts with "moviesearch".
    models::Query parse_rating_distance_line(const std::vector<std::string>& arguments);
}
