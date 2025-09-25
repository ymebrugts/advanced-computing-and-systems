#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file overlap.h
 * @date 2025-09-25
 */


#include <optional>
#include <string>
#include <vector>
#include <ostream>
#include <unordered_map>

#include "MovieRating.h"
#include "../models/Query.h"

namespace movie_rate::utils {
    void check_dataset_overlap(
        const std::unordered_map<int, std::vector<movie_parser::models::MovieRating>>& ratings_by_movie,
        int threshold = 5);

    void check_dataset_overlap_second(
        const std::unordered_map<int, std::vector<movie_parser::models::MovieRating>>& ratings_by_user,
        int threshold = 5);
}
