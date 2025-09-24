#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file movie_rate.h
 * @date 2025-09-24
 */


#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../models/MovieRating.h"
#include "../models/MovieRateResult.h"

namespace movie_parser::algorithms {

    std::optional<models::MovieRateResult> predict_movie_rate(
        int target_user_id,
        int target_movie_id,
        const std::unordered_map<int, std::vector<movie_parser::models::MovieRating>>& ratings_by_user);

}
